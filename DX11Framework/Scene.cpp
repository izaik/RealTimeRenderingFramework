#include "Scene.h"

Scene::Scene(ComPtr<ID3D11DeviceContext> &context, ComPtr<ID3D11Device> &device) {
	SceneRoot = new Node("SceneRoot");
	SceneContext = context;
	SceneDevice = device;
};