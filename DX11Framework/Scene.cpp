#include "Scene.h"
Scene::Scene()
{
	SceneRoot = std::make_unique<Node>("SceneRoot");
};

void Scene::DrawScene(RenderContext _RenderContext)
{
	//verify we have all needed data
	if (_RenderContext.context == nullptr || _RenderContext.cbData == nullptr)
	{
		return;
	}

	SceneRoot->UpdateWorldMatrix();
	SceneRoot->Draw(_RenderContext);
};

void Scene::LoadSceneFromJson(std::string ScenePath) {
	std::ifstream input_json(ScenePath);
	json j;
	input_json >> j;


};

