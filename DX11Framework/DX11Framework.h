#pragma once

#include <windows.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
#include <array>
#include "ScanCode.h"
#include "OBJLoader.h"
#include "Structures.h"
#include "GameObject.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct ConstantBuffer
{
	XMMATRIX Projection;
	XMMATRIX View;
	XMMATRIX World;
	Material mat;
	XMFLOAT3 camPos;
	Light light;
	float timeSinceStart;
};

class DX11Framework
{
	int _WindowWidth = 1280;
	int _WindowHeight = 768;

	ComPtr<ID3D11DeviceContext> _immediateContext;
	ComPtr<ID3D11Device> _device;
	ComPtr<IDXGIDevice> _dxgiDevice;
	ComPtr<IDXGIFactory2> _dxgiFactory;
	ComPtr<ID3D11RenderTargetView> _frameBufferView;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
	ComPtr<IDXGISwapChain1> _swapChain;
	D3D11_VIEWPORT _viewport;

	ComPtr<ID3D11RasterizerState> _fillState;
	ComPtr<ID3D11RasterizerState> _wireframeState;
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11InputLayout> _inputLayout;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11Buffer> _constantBuffer;
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;
	ComPtr<ID3D11SamplerState> _anisotropicSampler;

	HWND _windowHandle;

	XMFLOAT4X4 _World;
	XMFLOAT4X4 _View;
	XMFLOAT4X4 _Projection;
	Material _defaultMaterial;
	Light _loadedLight;

	
	ComPtr<ID3D11ShaderResourceView> _crateDiffuseTex;
	ComPtr<ID3D11ShaderResourceView> _crateSpecularTex;
	
	GameObject* objectTest = new GameObject(_World);

	ConstantBuffer _cbData;

	std::array<bool, SCANCODE_COUNT> _keys;

	float simpleCount = 0;
	float deltaTime = 0;

	float userRotate = 0;

public:
	void Initialise(HINSTANCE hInstance, int nCmdShow);
	void CreateWindowHandle(HINSTANCE hInstance, int nCmdShow);
	void CreateD3DDevice();
	void CreateSwapChainAndFrameBuffer();
	void CompileShader(const std::wstring& fileName, const std::string& entryPoint, const std::string& profile, ComPtr<ID3DBlob>& shaderBlob) const;
	void InitShadersAndInputLayout();
	void InitVertexIndexBuffers();
	void InitPipelineVariables();
	void InitRunTimeData();
	void InitImGui();

	void OnKeyDown(LPARAM lparam);
	void OnKeyUp(LPARAM lparam);

	~DX11Framework();

	LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Update();
	void Draw();
	void SetUpUI() const;
	void RenderUI();
};