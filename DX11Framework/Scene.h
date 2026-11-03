#pragma once

#include <wrl.h>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
#include "Node.h"
#include "Structures.h"

using Microsoft::WRL::ComPtr;

class Scene {
public:
	std::unique_ptr<Node> SceneRoot = nullptr;

	Scene();
	virtual ~Scene() = default;

	void UpdateNodeTransformFromJson(const json& j, Node* n, float simpleCount);
	void LoadSceneFromJson(std::string SceneName);
	void DrawScene(RenderContext _RenderContext);
};

