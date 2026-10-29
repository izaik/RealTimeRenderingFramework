#pragma once
#include "Structures.h"
#include <d3d11_4.h>
#include <wrl.h>
#include <vector>

using Microsoft::WRL::ComPtr;

class GameObject
{
	/*SCENE GRAPH*/
	std::vector<ComPtr<GameObject>> Children;
	ComPtr<GameObject> Parent = nullptr;
public:
	Transform transform; 

	Transform& GetTransform() { return transform; }

	template<typename... TArgs>
	void AddChild(const TArgs&... args) {
		Children.emplace_back(ComPtr<GameObject>(args...));
		Children.back()->Parent = this;
	}

	void UpdateSelfAndChild();
	XMMATRIX GetLocalObjectMatrix();
};

/*Class for objects using meshes and textures*/
class Model : GameObject {
	ComPtr<ID3D11ShaderResourceView> Diffuse;
	ComPtr<ID3D11ShaderResourceView> Specular;
	MeshData Mesh;

public:
	void SetDiffuse(ComPtr<ID3D11ShaderResourceView>& in, ComPtr<ID3D11DeviceContext> context);
	ComPtr<ID3D11ShaderResourceView>& GetDiffuse() { return Diffuse; }

	void SetSpecular(ComPtr<ID3D11ShaderResourceView>& in, ComPtr<ID3D11DeviceContext> context);
	ComPtr<ID3D11ShaderResourceView>& GetSpecular() { return Specular; }

	void SetMeshData(MeshData in) { Mesh = in; }
	MeshData* GetData() { return &Mesh; }

	void Draw(
		ConstantBuffer cbData,
		ComPtr<ID3D11DeviceContext> context,
		ComPtr<ID3D11Buffer> constantBuffer,
		D3D11_MAPPED_SUBRESOURCE mappedSubresource);

	void Update(float deltaTime);
};