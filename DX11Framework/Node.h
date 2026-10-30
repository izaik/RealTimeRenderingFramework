#pragma once

#include "Structures.h"
#include <vector>
#include <memory>
#include <algorithm>
//https://webglfundamentals.org/webgl/lessons/webgl-scene-graph.html link for template i based this off - include in report
//https://developers.google.com/sceneform/reference/com/google/ar/sceneform/NodeParent

using Microsoft::WRL::ComPtr;

class Node
{
	std::string Name;
public:
	DirectX::XMMATRIX LocalMatrix;
	DirectX::XMMATRIX WorldMatrix;
	std::vector<std::unique_ptr<Node>> Children;
	Node* Parent;

	Node(std::string name);
	virtual ~Node() = default; //use destructor to stop memory leaks

	template<typename T = Node, typename... TArgs>
	void AddChild(TArgs&... args) {
		Children.push_back(std::make_unique<T>(args)...); //use T instead of 
		Children.back()->Parent = this;
	};
	void UpdateWorldMatrix();
	//Node* FindNode(std::string name)
	//{
	//	// for each child
	//		// if child name == name
	//			return child_ptr.get()
	//}
};

/*Class for objects using meshes and textures*/
class GeometryNode : public Node {
	ComPtr<ID3D11ShaderResourceView> Diffuse;
	ComPtr<ID3D11ShaderResourceView> Specular;
	MeshData Mesh;

public:
	GeometryNode(std::string name) : Node(name) {}

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