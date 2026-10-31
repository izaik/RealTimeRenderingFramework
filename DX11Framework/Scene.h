#pragma once

#include <wrl.h>
#include "Node.h"
#include "Structures.h"

using Microsoft::WRL::ComPtr;

class Scene {
public:
	Node* SceneRoot;

	Scene();
	virtual ~Scene() = default;

	void DrawScene(RenderContext _RenderContext);
};