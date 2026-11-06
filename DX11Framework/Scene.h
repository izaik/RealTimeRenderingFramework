#pragma once

#include <wrl.h>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
#include "Node.h"
#include "Structures.h"
#include "ResourceManager.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Scene {
public:
	std::unique_ptr<Node> SceneRoot = nullptr;

	Scene();
	virtual ~Scene() = default;

	void ParseNodeRecursive(const json& jNode, Node* parentNode,
		ResourceManager* rm, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void UpdateNodeTransformFromJson(const json& jNode, Node* currentNode);
	void LoadSceneFromJson(std::string SceneName,
		ResourceManager* rm, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void DrawScene(RenderContext _RenderContext);
	void UpdateScene(float simpleCount);
};

