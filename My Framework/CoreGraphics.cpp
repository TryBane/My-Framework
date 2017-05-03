#include "CoreGraphics.h"
#include "Window.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <D3Dcompiler.h>
#include <fstream>
#include <array>

#pragma comment( lib, "d3d11.lib")
#pragma comment( lib, "D3DCompiler.lib")

// this function initializes and prepares Direct3D for use
CoreGraphics::CoreGraphics( WindowKey& key )
{
	assert( key.window != nullptr );

	// set up the swap chain description
	DXGI_SWAP_CHAIN_DESC scd = {0};
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = key.window;                          // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

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
	ID3D11Texture2D *pBackBuffer;
	if( FAILED( hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer) ) )
	{
		throw GRAPHICS_EXCEPTION ( hr, L"Getting Back Buffer");
	}

	// use the back buffer address to create the render target
	if( FAILED( hr = dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating Render Target View" );
	}
	pBackBuffer->Release();

	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);



	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

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
		{0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
		{-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}
	};


	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	HRESULT hr;

	if( FAILED( hr = dev->CreateBuffer(&bd, NULL, &pVBuffer) ) )       // create the buffer
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating Vertex Buffer" );
	}

												   // copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	if( FAILED( hr = devcon->Map(pVBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms) ) )    // map the buffer
	{
		throw GRAPHICS_EXCEPTION( hr,L"Mapping Vertex Buffer" );
	}
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	devcon->Unmap(pVBuffer.Get(), NULL);                                      // unmap the buffer

	/***********************************/
	/******* Initialize Pipeline *******/
	/***********************************/

	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	/*
	if( FAILED( hr = D3DCompile(0 ,0 ,"Shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Compiling Vertex Shader" );
	}
	if( FAILED( hr = D3DCompile(0 ,0 ,"Shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Compiling Pixel Shader" );
	}
	
	// encapsulate both shaders into shader objects
	if( FAILED( hr = dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating Vertex Shader" );
	}
	if( FAILED( hr = dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating Pixel Shader" );
	}

	// set the shader objects
	devcon->VSSetShader(pVS.Get(), 0, 0);
	devcon->PSSetShader(pPS.Get(), 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	//dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout.Get());
	*/
}

// this function performs updates to the state of the game
void CoreGraphics::Update()
{

}

// this function renders a single frame of 3D graphics
void CoreGraphics::Render()
{
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
