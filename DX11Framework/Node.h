#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <wrl.h>
#include "OBJLoader.h"
#include "Structures.h"
#include "Resource.h"
#include "ResourceManager.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

//https://webglfundamentals.org/webgl/lessons/webgl-scene-graph.html
//https://developers.google.com/sceneform/reference/com/google/ar/sceneform/NodeParent
//https://learnopengl.com/Guest-Articles/2021/Scene/Scene-Graph
//https://docs.unity.com/en-us/engine/6000.6/manual/working-with-gameobjects/game-objects

class Node
{
	std::string Name;
public:
	ResourceManager* RM;
	DirectX::XMFLOAT4X4 LocalMatrix;
	DirectX::XMFLOAT4X4 WorldMatrix;
	
	std::vector<std::unique_ptr<Node>> Children;
	Node* Parent;
	
	bool ContinuousRotation = 0;
	DirectX::XMFLOAT4 ContinuousRotationVector;

	Node(ResourceManager* rm, std::string name);
	virtual ~Node() = default; //use destructor to stop memory leaks

	template<typename T = Node, typename... TArgs>
	void AddChild(TArgs&&... args) { //&& is a universal reference to tell compiler to figure out if parameter is lvalue(normal variable) or rvalue(temp reference)
		Children.push_back(std::make_unique<T>(std::forward<TArgs>(args)...)); //use T instead of Node so it can be derived class too and use forward to turn any rvalues back to rvalues
		Children.back()->Parent = this;
	};
	void UpdateWorldMatrix(float simpleCount);
	virtual void Draw(RenderContext& _RenderContext);
};

/*Class for objects using meshes and textures*/
class GeometryNode : public Node {
	std::shared_ptr<TextureResource> Diffuse;
	std::shared_ptr<TextureResource> Specular;
	float Shininess = 128.0f;
	std::shared_ptr<MeshResource> MeshRes;

public:
	GeometryNode(ResourceManager* RM, std::string name,
		const char* diffpath, const char* specpath, float shininess, const char* meshpath, //there is probably a better way to do this
		ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);

	void Draw(RenderContext& _RenderContext) override;
};
// TODO: add more node types