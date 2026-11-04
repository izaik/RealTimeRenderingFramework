#include "Scene.h"

//validation function to safely get info from json
template <typename T>
T GetSafeValue(const nlohmann::json& jsonObject, const std::string& key, T defaultValue) {
	return jsonObject.contains(key) ? jsonObject[key].get<T>() : defaultValue;
};

Scene::Scene() {}; //Load scene from json will construct the scene

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

void Scene::ParseNodeRecursive(const json& jNode, Node* parentNode, float simpleCount,
	ResourceManager* rm, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) {
	std::string name = GetSafeValue<std::string>(jNode, "name", "Node");
	std::string type = GetSafeValue<std::string>(jNode, "type", "Node");

	Node* currentNode = nullptr;

	if (type == "GeometryNode") {
		std::string diffPath = GetSafeValue<std::string>(jNode, "diffuse", "");
		std::string specPath = GetSafeValue<std::string>(jNode, "specular", "");
		std::string meshPath = GetSafeValue<std::string>(jNode, "mesh", "");

		parentNode->AddChild<GeometryNode>(rm, name, diffPath.c_str(), specPath.c_str(), meshPath.c_str(), device, context);
		currentNode = parentNode->Children.back().get();
	}
	// TODO: handle more node types here

	// apply local transform from json
	UpdateNodeTransformFromJson(jNode, currentNode, simpleCount);

	// recursively read child nodes if present
	if (jNode.contains("children") && jNode["children"].is_array()) {
		for (const auto& jChild : jNode["children"]) {
			ParseNodeRecursive(jChild, currentNode, simpleCount, rm, device, context);
		}
	}
}

void Scene::UpdateNodeTransformFromJson(const json& jNode, Node* currentNode, float simpleCount) {
	const auto& transform = GetSafeValue(jNode, "transform", NULL);
	const auto& behavior = GetSafeValue(jNode, "behavior", NULL);

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

	// TODO: might need different logic to handle rotation that uses a continuously updated variable
	float currentPitch = basePitch + (simpleCount * rotationSpeedPitch);
	float currentYaw = baseYaw + (simpleCount * rotationSpeedYaw);
	float currentRoll = baseRoll + (simpleCount * rotationSpeedRoll);

	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(currentPitch, currentYaw, currentRoll);

	XMMATRIX LocalMatrix =
		XMMatrixScalingFromVector(scale) *
		RotationMatrix *
		XMMatrixTranslationFromVector(translation);

	XMStoreFloat4x4(&currentNode->LocalMatrix, LocalMatrix);
}

void Scene::LoadSceneFromJson(std::string ScenePath, float simpleCount,
	ResourceManager* rm, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) {
	std::ifstream input_json(ScenePath);

	json j;
	input_json >> j;

	//scene root node is created here
	SceneRoot = std::make_unique<Node>(rm, "SceneRoot");

	//the top level elements in the json are the top level nodes directly attached to the sceneroot
	if (j.is_array()) {
		for (const auto& jNode : j) {
			ParseNodeRecursive(jNode, SceneRoot.get(), simpleCount, rm, device, context);
		}
	}
};

