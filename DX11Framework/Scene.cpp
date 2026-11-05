#include "Scene.h"

Scene::Scene() {}; //Load scene from json will construct the scene

void Scene::DrawScene(RenderContext _RenderContext)
{
	//verify we have all needed data
	if (_RenderContext.context == nullptr || _RenderContext.cbData == nullptr)
	{
		return;
	}

	//SceneRoot->UpdateWorldMatrix(simpleCount);
	SceneRoot->Draw(_RenderContext);
};

void Scene::UpdateScene(float simpleCount) {
	SceneRoot->UpdateWorldMatrix(simpleCount);
};

void Scene::ParseNodeRecursive(const json& jNode, Node* parentNode, float simpleCount,
	ResourceManager* rm, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) {
	std::string name = jNode.value("name", "Node");
	std::string type = jNode.value("type", "Node");

	Node* currentNode = nullptr;

	if (type == "GeometryNode") {
		std::string diffPath = jNode.value("diffpath", "");
		std::string specPath = jNode.value("specpath", "");
		std::string meshPath = jNode.value("meshpath", "");

		parentNode->AddChild<GeometryNode>(rm, name, diffPath.c_str(), specPath.c_str(), meshPath.c_str(), device, context);
		currentNode = parentNode->Children.back().get();
	}
	// TODO: handle more node types here

	// apply local transform from json
	UpdateNodeTransformFromJson(jNode, currentNode, simpleCount);

	// recursively read child nodes if present
	if (jNode.contains("children") && jNode["children"].is_array()) {
		for (const json& jChild : jNode["children"]) {
			ParseNodeRecursive(jChild, currentNode, simpleCount, rm, device, context);
		}
	}
}

void Scene::UpdateNodeTransformFromJson(const json& jNode, Node* currentNode, float simpleCount) {
	json jTransform = jNode.value("transform", nlohmann::json::object());
	json jBehavior = jNode.value("behavior", nlohmann::json::object());

	json jScale = jTransform.value("scale", nlohmann::json::object());
	json jTranslation = jTransform.value("position", nlohmann::json::object());
	json jRotation = jTransform.value("rotation", nlohmann::json::object());

	//get base scale and translation vectors from json
	XMVECTOR scale = XMVectorSet(jScale["x"], jScale["y"], jScale["z"], 0.0f);
	XMVECTOR translation = XMVectorSet(jTranslation["x"], jTranslation["y"], jTranslation["z"], 1.0f);

	//get base rotation vectors from json
	float basePitch = DirectX::XMConvertToRadians(jRotation["pitch"]);
	float baseYaw = DirectX::XMConvertToRadians(jRotation["yaw"]);
	float baseRoll = DirectX::XMConvertToRadians(jRotation["roll"]);

	// TODO: If statement to set flag on node to rotate
	if (!jRotation.empty()) {
		// get the speed of rotation
		float rotationSpeedPitch = DirectX::XMConvertToRadians(jBehavior.value("rotationSpeedPitch", 0.0));
		float rotationSpeedYaw = DirectX::XMConvertToRadians(jBehavior.value("rotationSpeedYaw", 0.0));
		float rotationSpeedRoll = DirectX::XMConvertToRadians(jBehavior.value("rotationSpeedRoll", 0.0));

		currentNode->ContinuousRotation = 1;
		XMMATRIX ContinuousRotationMatrix = XMMatrixRotationRollPitchYaw(rotationSpeedPitch, rotationSpeedYaw, rotationSpeedRoll);
		// TODO: might need different logic to handle rotation that uses a continuously updated variable
		XMStoreFloat4x4(&currentNode->ContinuousRotationMatrix, ContinuousRotationMatrix);
	}

	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(basePitch, baseYaw, baseRoll);

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

