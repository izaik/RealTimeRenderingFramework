#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "OBJLoader.h"
#include "Structures.h"
#include "Resource.h"
//https://webglfundamentals.org/webgl/lessons/webgl-scene-graph.html
//https://developers.google.com/sceneform/reference/com/google/ar/sceneform/NodeParent
//https://learnopengl.com/Guest-Articles/2021/Scene/Scene-Graph
//https://docs.unity.com/en-us/engine/6000.6/manual/working-with-gameobjects/game-objects

using Microsoft::WRL::ComPtr;

class Node
{
	std::string Name;
public:
	DirectX::XMFLOAT4X4 LocalMatrix;
	DirectX::XMFLOAT4X4 WorldMatrix;
	std::vector<std::unique_ptr<Node>> Children;
	Node* Parent;

	Node(std::string name);
	virtual ~Node() = default; //use destructor to stop memory leaks

	template<typename T = Node, typename... TArgs>
	void AddChild(TArgs&&... args) { //&& is a universal reference to tell compiler to figure out if parameter is lvalue(normal variable) or rvalue(temp reference)
		Children.push_back(std::make_unique<T>(std::forward<TArgs>(args)...)); //use T instead of Node so it can be derived class too and use forward to turn any rvalues back to rvalues
		Children.back()->Parent = this;
	};
	void UpdateWorldMatrix();
	//Node* FindNode(std::string name)
	//{
	//	// for each child
	//		// if child name == name
	//			return child_ptr.get()
	//}
	virtual void Draw(RenderContext& _RenderContext);
};

/*Class for objects using meshes and textures*/
class GeometryNode : public Node {
	TextureResource Diffuse;
	TextureResource Specular;
	MeshData Mesh;

public:
	GeometryNode(std::string name,
		TextureResource& diffuse,
		TextureResource& specular,
		const char* filepath, ComPtr<ID3D11Device>& device);

	void Draw(RenderContext& _RenderContext) override;
};