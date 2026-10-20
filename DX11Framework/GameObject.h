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
	void SetTexture(ComPtr<ID3D11ShaderResourceView> in) { _Texture = in; }
	ComPtr<ID3D11ShaderResourceView> & GetTexture() { return _Texture; }

	void SetMeshData(MeshData in) { _Mesh = in; }
	MeshData* GetData() { return &_Mesh; }

	void Draw(ComPtr<ID3D11DeviceContext> context);

	void Update(float deltaTime);
};