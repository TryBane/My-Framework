#include "CoreGraphics.h"
#include "Window.h"
#include <fstream>
#include <array>
#include <vector>

#pragma comment( lib, "d3d11.lib")

// this function loads a file into a vector<char>
std::vector<byte> LoadShaderFile(std::string File)
{
	std::vector<byte> FileData;

	// open the file
	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);

	// if open was successful
	if (VertexFile.is_open())
	{
		// find the length of the file
		int Length = (int)VertexFile.tellg();

		// collect the file data
		FileData = std::vector<byte>(Length);
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(reinterpret_cast<char*>(FileData.data()), Length);
		VertexFile.close();
	}

	return FileData;
}

// this function initializes and prepares Direct3D for use
CoreGraphics::CoreGraphics( WindowKey& key )
{
	assert( key.window != nullptr );

	// set up the swap chain description
	DXGI_SWAP_CHAIN_DESC scd = {0};
	scd.BufferCount = 1;									// one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// how swap chain is to be used
	scd.OutputWindow = key.window;							// the window to be used
	scd.SampleDesc.Count = 4;								// how many multisamples
	scd.Windowed = TRUE;									// windowed/full-screen mode

	HRESULT hr;
	// Create the device and device context objects
	if( FAILED( hr = D3D11CreateDevice( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0u,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&dev,
		nullptr,
		&devcon ) ) )
	{
		throw GRAPHICS_EXCEPTION( hr, L"Creating Device");
	}


	// obtain the DXGI factory
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;

	dev.As(&dxgiDevice);
	dxgiDevice->GetAdapter(&dxgiAdapter);
	dxgiAdapter->GetParent( __uuidof(IDXGIFactory),&dxgiFactory );

	if( FAILED( hr = dxgiFactory->CreateSwapChain(
		dev.Get(),
		&scd,
		&swapchain) ) )
	{
		throw GRAPHICS_EXCEPTION( hr, L"Creating Swap Chain");
	}

	// get the address of the back buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	if( FAILED( hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer) ) )
	{
		throw GRAPHICS_EXCEPTION ( hr, L"Getting Back Buffer");
	}

	// use the back buffer address to create the render target
	if( FAILED( hr = dev->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &backbuffer) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating Render Target View" );
	}


	// Set the viewport
	D3D11_VIEWPORT viewport = { 0 };

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = ScreenWidth;
	viewport.Height = ScreenHeight;

	devcon->RSSetViewports(1, &viewport);

	Initialize();
}

CoreGraphics::~CoreGraphics()
{
	swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

	if(devcon)
	{
		devcon->ClearState();
	}

}

void CoreGraphics::Initialize()
{
	/***********************************/
	/******* Initialize Graphics *******/
	/***********************************/

	// create a triangle using the VERTEX struct
	VERTEX OurVertices[] =
	{
		{-0.25f, 0.5f, 0.0f, {1.0f, 0.0f, 0.0f, 1.0f}},
		{0.20f, -0.5, 0.0f, {0.0f, 1.0f, 0.0f, 1.0f} },
		{-0.70f, -0.5f, 0.0f, {0.0f, 0.0f, 1.0f, 1.0f}}
	};


	// create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };

	bd.Usage = D3D11_USAGE_DYNAMIC;																	// write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 3;																// size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;														// use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;														// allow CPU to write in buffer

	HRESULT hr;

	if( FAILED( hr = dev->CreateBuffer(&bd, NULL, &pVBuffer) ) )									// create the buffer
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating Vertex Buffer" );
	}


	D3D11_MAPPED_SUBRESOURCE ms;
	if( FAILED( hr = devcon->Map(pVBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms) ) )		// map the buffer
	{
		throw GRAPHICS_EXCEPTION( hr,L"Mapping Vertex Buffer" );
	}
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));												// copy the data
	devcon->Unmap(pVBuffer.Get(), NULL);															// unmap the buffer

	/***********************************/
	/******* Initialize Pipeline *******/
	/***********************************/

	// load the shader files
	std::vector<byte> VSFile = LoadShaderFile("VertexShader.cso");
	std::vector<byte> PSFile = LoadShaderFile("PixelShader.cso");

	// create the shader objects
	if( FAILED( hr = dev->CreateVertexShader(VSFile.data(), VSFile.size(), nullptr, &pVS) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating the Vertex Shader" );
	}
	if( FAILED( hr = dev->CreatePixelShader(PSFile.data(), PSFile.size(), nullptr, &pPS) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating the Pixel Shader" );
	}

	// set the shader objects as the active shaders
	devcon->VSSetShader(pVS.Get(), nullptr, 0);
	devcon->PSSetShader(pPS.Get(), nullptr, 0);

	// initialize input layout
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// create and set the input layout
	if( FAILED( hr = dev->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile.data(), VSFile.size(), &pLayout) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating the Input Layout" );
	}
	devcon->IASetInputLayout(pLayout.Get());
}

// this function performs updates to the state of the game
void CoreGraphics::Update()
{

}

// this function renders a single frame of 3D graphics
void CoreGraphics::Render()
{
	// set our new render target object as the active render target
	devcon->OMSetRenderTargets(1, backbuffer.GetAddressOf(), nullptr);

	// clear the back buffer to a deep blue
	float color[4] = {0.0f, 0.2f, 0.4f, 1.0f};
	devcon->ClearRenderTargetView(backbuffer.Get(), color);

	// set the vertex buffer
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, pVBuffer.GetAddressOf(), &stride, &offset);

	// set the primitive topology
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw 3 vertices, starting from vertex 0
	devcon->Draw(3, 0);

	HRESULT hr;
	// switch the back buffer and the front buffer
	if( FAILED( hr = swapchain->Present(1, 0) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Swapping Back and Front Buffers" );
	}
}

CoreGraphics::GraphicsException::GraphicsException( HRESULT hr,const std::wstring & error,const wchar_t * fileName,unsigned int lineFound )
	:
	MyException( fileName, lineFound, error ),
	hr( hr )
{}

std::wstring CoreGraphics::GraphicsException::GetErrorMessage() const
{
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorType();
	const std::wstring errorDesc = GetErrorInfo();
	const std::wstring& note = GetError();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
		: empty)
		+ (!errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
			: empty)
		+ (!note.empty() ? std::wstring( L"Error Occurred: " ) + note + L"\n"
			: empty)
		+ (!location.empty() ? std::wstring( L"Location: " ) + location
			: empty);
}

std::wstring CoreGraphics::GraphicsException::GetErrorType( ) const
{
	return DXGetErrorString( hr );
}

std::wstring CoreGraphics::GraphicsException::GetErrorInfo( ) const
{
	std::array<wchar_t,512> wideDescription;
	DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
	return wideDescription.data();

}

std::wstring CoreGraphics::GraphicsException::GetExceptionType( ) const
{
	return L"Core Graphics Exception";
}