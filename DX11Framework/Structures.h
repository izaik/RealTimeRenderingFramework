#pragma once

#include <d3d11_4.h>
#include <DirectXMath.h>
#include <wrl.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct SimpleVertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;

	bool operator<(const SimpleVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	};
};

struct MeshData
{
	ComPtr<ID3D11Buffer> VertexBuffer;
	ComPtr<ID3D11Buffer> IndexBuffer;
	UINT VBStride;
	UINT VBOffset;
	UINT IndexCount;
};

struct Material
{
	XMFLOAT4 Diffuse;
	XMFLOAT4 Ambient;
	XMFLOAT4 Specular;
};

struct Light
{
	XMFLOAT4 Diffuse;
	XMFLOAT4 Ambient;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
};

struct ConstantBuffer
{
	XMMATRIX Projection;
	XMMATRIX View;
	XMMATRIX World;
	XMFLOAT3 camPos;
	float Shininess = 0.0f;
	Light light;
	float timeSinceStart;
};

/*Struct used for commonly used render data*/
struct RenderContext {
	ID3D11DeviceContext* context = nullptr;
	ID3D11Buffer* constantBuffer = nullptr;
	ConstantBuffer* cbData = nullptr;
	
	ID3D11RasterizerState* _fillState = nullptr;
	ID3D11DepthStencilState* _defaultDepthStencil = nullptr;
	ID3D11VertexShader* _vertexShader = nullptr;
	ID3D11PixelShader* _pixelShader = nullptr;

	ID3D11RasterizerState* _skyboxState = nullptr;
	ID3D11DepthStencilState* _depthStencilSkybox = nullptr;
	ID3D11VertexShader* _vertexShaderSkybox = nullptr;
	ID3D11PixelShader* _pixelShaderSkybox = nullptr;
};