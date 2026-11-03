#include "Scene.h"

//validation function to safely get info from json
template <typename T>
T GetSafeValue(const nlohmann::json& jsonObject, const std::string& key, T defaultValue) {
	return jsonObject.contains(key) ? jsonObject[key].get<T>() : defaultValue;
};

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

void Scene::UpdateNodeTransformFromJson(const json& j, Node* n, float simpleCount) {
	const auto& transform = GetSafeValue(j, "transform", NULL);
	const auto& behavior = GetSafeValue(j, "behavior", NULL);

	//get base scale and translation vectors from json
	XMVECTOR scale = XMVectorSet(transform["scale"]["x"], transform["scale"]["y"], transform["scale"]["z"], 0.0f);
	XMVECTOR translation = XMVectorSet(transform["translation"]["x"], transform["translation"]["y"], transform["translation"]["z"], 1.0f);

	//get base rotation vectors from json
	float basePitch = transform["rotation"]["pitch"];
	float baseYaw = transform["rotation"]["yaw"];
	float baseRoll = transform["rotation"]["roll"];

	//get the speed of rotation
	float rotationSpeedPitch = GetSafeValue(behavior, "rotationSpeedPitch", NULL);
	float rotationSpeedYaw = GetSafeValue(behavior, "rotationSpeedYaw", NULL);
	float rotationSpeedRoll = GetSafeValue(behavior, "rotationSpeedRoll", NULL);

	float currentPitch = basePitch + (simpleCount * rotationSpeedPitch);
	float currentYaw = baseYaw + (simpleCount * rotationSpeedYaw);
	float currentRoll = baseRoll + (simpleCount * rotationSpeedRoll);

	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(currentPitch, currentYaw, currentRoll);

	XMMATRIX LocalMatrix =
		XMMatrixScalingFromVector(scale) *
		RotationMatrix *
		XMMatrixTranslationFromVector(translation);

	XMStoreFloat4x4(&n->LocalMatrix, LocalMatrix);
}

void Scene::LoadSceneFromJson(std::string ScenePath) {
	std::ifstream input_json(ScenePath);
	json j;
	input_json >> j;


};

