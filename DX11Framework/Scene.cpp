#include "Scene.h"

Scene::Scene() {
	SceneRoot = new Node("SceneRoot");
};

void Scene::DrawScene(RenderContext _RenderContext)
{
	SceneRoot->Draw(_RenderContext);
};