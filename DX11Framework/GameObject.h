#pragma once
#include "Structures.h"
#include <d3d11_4.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class GameObject
{
	DirectX::XMFLOAT4X4 _World;
	ComPtr<ID3D11ShaderResourceView> _Texture;
	MeshData _Mesh;

public:
	GameObject(DirectX::XMFLOAT4X4 world);

	void SetTexture(ComPtr<ID3D11ShaderResourceView> in) { _Texture = in; }
	ComPtr<ID3D11ShaderResourceView>& GetTexture() { return _Texture; }

	void SetMeshData(MeshData in) { _Mesh = in; }
	MeshData* GetData() { return &_Mesh; }

	void Draw(
		ConstantBuffer cbData,
		ComPtr<ID3D11DeviceContext> context,
		ComPtr<ID3D11Buffer> constantBuffer,
		D3D11_MAPPED_SUBRESOURCE mappedSubresource);

	void Update(float deltaTime);
};