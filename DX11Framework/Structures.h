#pragma once

#include <d3d11_4.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

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
	float Shininess;
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
	Material mat;
	XMFLOAT3 camPos;
	Light light;
	float timeSinceStart;
};

struct Transform {
	/*SPACE INFORMATION*/
	//Local Space Info
	XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 EulerRot = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 Scale = { 1.0f, 1.0f, 1.0f };

	//Global Space Info
	XMMATRIX Model = XMMatrixSet(
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
};