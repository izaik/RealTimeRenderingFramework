#include "Scene.h"

Scene::Scene() {
	SceneRoot = new Node("SceneRoot");
};

void Scene::DrawScene(RenderContext _RenderContext)
{
	//verify we have all needed data
	if (SceneRoot == nullptr || _RenderContext.context == nullptr || _RenderContext.cbData == nullptr)
	{
		return;
	}

	SceneRoot->UpdateWorldMatrix();
	SceneRoot->Draw(_RenderContext);
};