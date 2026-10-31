#pragma once

#include <wrl.h>
#include "Node.h"
#include "Structures.h"

using Microsoft::WRL::ComPtr;

class Scene {
public:
	std::unique_ptr<Node> SceneRoot = nullptr;

	Scene();
	virtual ~Scene() = default;

	void DrawScene(RenderContext _RenderContext);
};