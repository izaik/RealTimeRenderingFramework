#pragma once
#include "Structures.h"
#include <d3d11_4.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include <algorithm>
//https://webglfundamentals.org/webgl/lessons/webgl-scene-graph.html link for template i based this off - include in report

using Microsoft::WRL::ComPtr;

class Node
{
	std::string Name;
public:
	DirectX::XMMATRIX LocalMatrix;
	DirectX::XMMATRIX WorldMatrix;
	std::vector<std::unique_ptr<Node>> Children;
	std::unique_ptr<Node> Parent;

	Node(std::string name, Node* parent) {
		Name = name;
		LocalMatrix = XMMatrixIdentity();
		WorldMatrix = XMMatrixIdentity();
		Children = {};
		std::unique_ptr<Node> Parent = std::make_unique<Node>(parent);
	}

	void SetParent(Node* parent);

	void UpdateWorldMatrix();
	//Node* FindNode(std::string name)
	//{
	//	// for each child
	//		// if child name == name
	//			return child_ptr.get()
	//}
};

/*Class for objects using meshes and textures*/
class GeometryNode : Node {
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