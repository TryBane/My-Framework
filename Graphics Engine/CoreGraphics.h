#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "My Exceptions.h"
#include "DXerr.h"
#include <DirectXMath.h>
#include <DirectXColors.h>
#include "Keyboard.h"

#define GRAPHICS_EXCEPTION( hr, error ) CoreGraphics::GraphicsException( hr, error, _CRT_WIDE(__FILE__),__LINE__)

// a struct to represent a single vertex
struct VERTEX
{
	DirectX::XMVECTORF32 position;			// vertex position
	DirectX::XMVECTORF32 normal;			// vertex normal direction
	DirectX::XMVECTORF32 color;				// vertex color
	DirectX::XMFLOAT2 U, V;
};

struct CBUFFER
{
	DirectX::XMMATRIX Final;
	DirectX::XMMATRIX Rotation;
	DirectX::XMVECTOR DiffuseVector;
	DirectX::XMVECTOR DiffuseColor;
	DirectX::XMVECTOR AmbientColor;
};

// a struct defining the contents of our constant buffer
struct OFFSET
{
	float X, Y, Z, W;
};

class CoreGraphics
{
public:
	class GraphicsException : public MyException
	{
	public:
		GraphicsException( HRESULT hr, const std::wstring& error, const wchar_t* fileName, unsigned int lineFound);
		std::wstring GetErrorType() const;
		std::wstring GetErrorInfo() const;
		std::wstring GetErrorMessage() const override;
		std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
public:
	CoreGraphics( class WindowKey& key,Keyboard& keyboard );
	CoreGraphics( const CoreGraphics& ) = delete;
	CoreGraphics& operator=( const CoreGraphics& ) = delete;
	~CoreGraphics();
	void Update();
	void Render();
	void AddVertices( std::vector<VERTEX> newVertices );
private:
	void SetMatrix();
	void Initialize();
	void SetStates();
	void DebugModeToggle();
	void LoadResources();
private:
	Microsoft::WRL::ComPtr<ID3D11Device> dev;					// the device interface
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> devcon;			// the device context interface
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;			// the swap chain interface
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pLayout;			// the pointer to the input layout
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVS;				// the pointer to the vertex shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPS;				// the pointer to the pixel shader
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVBuffer;				// the pointer to the vertex buffer
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backbuffer;	// the pointer to our back buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantbuffer;        // the constant buffer interface
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> zbuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexbuffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> defaultRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> wireFrameRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

public:
	static constexpr int ScreenOriginx = 300;
	static constexpr int ScreenOriginy = 100;
	static constexpr int ScreenWidth = 1200u;
	static constexpr int ScreenHeight = 900u;

private:
	std::vector<VERTEX> Vertices;
	DirectX::XMMATRIX finalMatrix;
	OFFSET Offset;
	Keyboard& keyboard;
	CBUFFER cBuffer;

	float rotateX = 0.0f;
	float rotateY = 0.0f;
	float rotateZ = 0.0f;

	float positionX =  1.5f;
	float positionY =  1.5f;
	float positionZ = -4.0f;

	bool debugModeToggle = false;
};