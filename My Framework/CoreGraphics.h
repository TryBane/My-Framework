#pragma once
#include <d3d11.h>
#include <d3dx10.h>
#include <wrl.h>
#include <vector>


// a struct to represent a single vertex
struct VERTEX
{
	float X, Y, Z;
	D3DXCOLOR Color;    // vertex position
};

class CoreGraphics
{
public:
	CoreGraphics( class WindowKey& key );
	CoreGraphics( const CoreGraphics& ) = delete;
	CoreGraphics& operator=( const CoreGraphics& ) = delete;
	~CoreGraphics();
	void Initialize();
	void Update();
	void Render();
private:
	Microsoft::WRL::ComPtr<ID3D11Device> dev;              // the device interface
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon;    // the device context interface
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;      // the swap chain interface
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pLayout;            // the pointer to the input layout
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVS;               // the pointer to the vertex shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPS;                // the pointer to the pixel shader
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVBuffer;                // the pointer to the vertex buffer
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backbuffer;    // the pointer to our back buffer

public:
	static constexpr int ScreenOriginx = 300;
	static constexpr int ScreenOriginy = 300;
	static constexpr int ScreenHeight = 600u;
	static constexpr int ScreenWidth = 800u;

};
