#pragma once

#include "Node.h"

using Microsoft::WRL::ComPtr;

class Scene {
public:
	Node* SceneRoot;
	ComPtr<ID3D11DeviceContext> SceneContext;
	ComPtr<ID3D11Device> SceneDevice;

	Scene(ComPtr<ID3D11DeviceContext> &context, ComPtr<ID3D11Device> &device);
	virtual ~Scene() = default;
};