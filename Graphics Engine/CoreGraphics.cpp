#include "CoreGraphics.h"
#include "Window.h"
#include <fstream>
#include <array>
#include <vector>
#include <iterator>
#include "VertexList.h"
#pragma comment( lib, "d3d11.lib")

using namespace DirectX::Colors;

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
CoreGraphics::CoreGraphics( WindowKey& key,Keyboard& Keyboard )
	:
	keyboard( Keyboard )
{

	Offset.X = 0.5f;
	Offset.Y = 0.2f;
	Offset.Z = 0.7f;
	Offset.W = 0.0f;

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


	// create a zbuffer
	D3D11_TEXTURE2D_DESC texd = {0};

	texd.Width = ScreenWidth;
	texd.Height = ScreenHeight;
	texd.ArraySize = 1;
	texd.MipLevels = 1;
	texd.SampleDesc.Count = 1;
	texd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> zbuffertexture;
	if( FAILED( hr = dev->CreateTexture2D( &texd,nullptr,&zbuffertexture ) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating zbuffer Texture" );
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));

	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	if( FAILED( hr = dev->CreateDepthStencilView(zbuffertexture.Get(), &dsvd, &zbuffer) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating Depth Stencil View" );
	}

	// Set the viewport
	D3D11_VIEWPORT viewport = { 0 };

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = ScreenWidth;
	viewport.Height = ScreenHeight;
	viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
	viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0

	devcon->RSSetViewports(1, &viewport);

	AddVertices( OurVertices );

	Initialize();

	dTime = 0.0f;

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
	HRESULT hr;

	/***********************************/
	/******* Initialize Graphics *******/
	/***********************************/
	
	// Index list for Cube
	std::vector<short> CubeVertexIndices =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};

	// create the index buffer
	D3D11_BUFFER_DESC ibd = {0};
	ibd.ByteWidth = sizeof(short) * CubeVertexIndices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA isrd = {&CubeVertexIndices[0], 0, 0};


	if( FAILED( hr = dev->CreateBuffer(&ibd, &isrd, &indexbuffer) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating the Index Buffer" );
	}

	// create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = (UINT)( sizeof(VERTEX) * Vertices.size() );
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = {&Vertices[0], 0, 0};

	if( FAILED( hr = dev->CreateBuffer( &bd,&srd,&pVBuffer ) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating the Vertex Buffer");
	}

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

	// initialize input layout
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// create and set the input layout
	if( FAILED( hr = dev->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile.data(), VSFile.size(), &pLayout) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating the Input Layout" );
	}

	bd = { 0 };
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBUFFER);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// Create Constant Buffer
	if( FAILED( hr = dev->CreateBuffer(&bd, nullptr, &constantbuffer) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Creating the Constant Buffer" );
	}
}

void CoreGraphics::SetMatrix()
{
	// calculate the world transformation
	DirectX::XMMATRIX matWorld = DirectX::XMMatrixRotationY(dTime);

	// calculate the view transformation
	DirectX::XMVECTOR vecCamPosition = DirectX::XMVectorSet(1.5f, 1.5f, -4.0f, 0);
	DirectX::XMVECTOR vecCamLookAt = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMVECTOR vecCamUp = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMMATRIX matView = DirectX::XMMatrixLookAtLH(vecCamPosition, vecCamLookAt, vecCamUp);

	// calculate the projection transformation
	DirectX::XMMATRIX matProjection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(45),                                      // the field of view
		(float)ScreenWidth / (float)ScreenHeight,  // aspect ratio
		1,                                                           // the near view-plane
		100);                                                        // the far view-plane

																	 // calculate the final matrix
	finalMatrix = matWorld * matView * matProjection;

	cBuffer.Final = finalMatrix;
	cBuffer.Rotation = matWorld;
	cBuffer.DiffuseVector = DirectX::XMVectorSet( 1.0f, 1.0f, 1.0f, 1.0f );
	cBuffer.DiffuseColor = DirectX::XMVectorSet( 0.5f, 0.5f, 0.5f, 1.0f );
	cBuffer.AmbientColor = DirectX::XMVectorSet( 0.2f, 0.2f, 0.2f, 1.0f );
}

// this function performs updates to the state of the game
void CoreGraphics::Update()
{
	dTime += 0.02f;
	if( keyboard.KeyIsPressed( VK_RIGHT ) )
	{
		dTime += 0.1f;
	}
	if( keyboard.KeyIsPressed( VK_LEFT ) )
	{
		dTime -= 0.1f;
	}
	if( keyboard.KeyIsPressed( VK_UP ) )
	{
		Offset.Y += 0.01f;
	}
	if( keyboard.KeyIsPressed( VK_DOWN ) )
	{
		Offset.Y -= 0.01f;
	}
	if( keyboard.KeyIsPressed( VK_SPACE ) )
	{
		Offset.Z -= 0.01f;
	}
	if( keyboard.KeyIsPressed( VK_SHIFT ) )
	{
		Offset.Z += 0.1f;
	}
}

// this function renders a single frame of 3D graphics
void CoreGraphics::Render()
{


	// set our new render target object as the active render target
	devcon->OMSetRenderTargets(1, backbuffer.GetAddressOf(), nullptr);

	// clear the back buffer to a deep blue
	float color[4] = {0.0f, 0.2f, 0.4f, 1.0f};
	devcon->ClearRenderTargetView(backbuffer.Get(), color);
	// clear the depth buffer
	devcon->ClearDepthStencilView(zbuffer.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);


	// Set start and end for Vertex Drawing

	// set the vertex buffer
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	SetMatrix();

	// set the shader objects as the active shaders
	// define the contents to be stored in the constant buffer
	// load the data into the constant buffer
	// set the primitive topology
	// draw all vertices from start->end
	devcon->VSSetShader(pVS.Get(), nullptr, 0);
	devcon->PSSetShader(pPS.Get(), nullptr, 0);
	devcon->VSSetConstantBuffers(0, 1, constantbuffer.GetAddressOf());
	// load the data into the constant buffer
	devcon->UpdateSubresource(constantbuffer.Get(), 0, 0, &cBuffer, 0, 0);
	devcon->IASetVertexBuffers(0, 1, pVBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetIndexBuffer(indexbuffer.Get(),DXGI_FORMAT_R16_UINT,0 );
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed( 36,0,0 );
	devcon->IASetInputLayout(pLayout.Get());

	HRESULT hr;

	// switch the back buffer and the front buffer
	if( FAILED( hr = swapchain->Present(1, 0) ) )
	{
		throw GRAPHICS_EXCEPTION( hr,L"Swapping Back and Front Buffers" );
	}
}

void CoreGraphics::AddVertices( std::vector<VERTEX> newVertices )
{
	for( auto i : newVertices )
	{
		Vertices.push_back( i );
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