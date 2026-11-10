#include "Scene.h"

Scene::Scene() {}; //Load scene from json will construct the scene

void Scene::DrawScene(RenderContext _RenderContext)
{
	//verify we have all needed data
	if (_RenderContext.context == nullptr || _RenderContext.cbData == nullptr)
	{
		return;
	}

	_RenderContext.context->VSSetShader(_RenderContext._vertexShaderSkybox, nullptr, 0);
	_RenderContext.context->PSSetShader(_RenderContext._pixelShaderSkybox, nullptr, 0);
	if (SceneSkybox != nullptr)
	{
		SceneSkybox->Draw(_RenderContext);
	}

	_RenderContext.context->VSSetShader(_RenderContext._vertexShader, nullptr, 0);
	_RenderContext.context->PSSetShader(_RenderContext._pixelShader, nullptr, 0);
	if (SceneRoot != nullptr)
	{
		SceneRoot->Draw(_RenderContext);
	}

	
};

void Scene::UpdateScene(float simpleCount) {
	SceneRoot->UpdateWorldMatrix(simpleCount);
};

void Scene::ParseNodeRecursive(const json& jNode, Node* parentNode,
	ResourceManager* rm, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) {
	std::string name = jNode.value("name", "Node");
	std::string type = jNode.value("type", "Node");

	Node* currentNode = nullptr;

	if (type == "GeometryNode") {
		std::string diffPath = jNode.value("diffpath", "");
		std::string specPath = jNode.value("specpath", "");
		float shininess = jNode.value("shininess", 0.0f);
		std::string meshPath = jNode.value("meshpath", "");

		parentNode->AddChild<GeometryNode>(rm, name, diffPath.c_str(), specPath.c_str(), shininess, meshPath.c_str(), device, context);
	}
	if (type == "LightNode") {
		json jDiffuse = jNode.value("diffuse", nlohmann::json::object());
		json jAmbient = jNode.value("ambient", nlohmann::json::object());
		json jSpecular = jNode.value("specular", nlohmann::json::object());
		json jDirection = jNode.value("direction", nlohmann::json::object());

		XMFLOAT4 diffuse = XMFLOAT4(jDiffuse["r"], jDiffuse["g"], jDiffuse["b"], jDiffuse["a"]);
		XMFLOAT4 ambient = XMFLOAT4(jAmbient["r"], jAmbient["g"], jAmbient["b"], jAmbient["a"]);
		XMFLOAT4 specular = XMFLOAT4(jSpecular["r"], jSpecular["g"], jSpecular["b"], jSpecular["a"]);
		XMFLOAT3 direction = XMFLOAT3(jDirection["x"], jDirection["y"], jDirection["z"]);

		parentNode->AddChild<LightNode>(rm, name, diffuse, ambient, specular, direction);
	}
	if (type == "SkyboxNode") {
		std::string jSkyboxTex = jNode.value("texturepath", "");
		SceneSkybox = new SkyboxNode(rm, name, jSkyboxTex.c_str(), device, context);
	}
	// TODO: handle more node types here

	//skybox nodes cannot have children
	if (type != "SkyboxNode") {
		// apply local transform from json
		currentNode = parentNode->Children.back().get();
		UpdateNodeTransformFromJson(jNode, currentNode);

		// recursively read child nodes if present
		if (jNode.contains("children") && jNode["children"].is_array()) {
			for (const json& jChild : jNode["children"]) {
				ParseNodeRecursive(jChild, currentNode, rm, device, context);
			}
		}
	}
}

void Scene::UpdateNodeTransformFromJson(const json& jNode, Node* currentNode) {
	json jTransform = jNode.value("transform", nlohmann::json::object());
	json jBehavior = jNode.value("behavior", nlohmann::json::object());

	json jScale = jTransform.value("scale", nlohmann::json::object());
	json jTranslation = jTransform.value("position", nlohmann::json::object());
	json jOrientation = jTransform.value("orientation", nlohmann::json::object());

	//get base scale and translation vectors from json
	XMVECTOR scale = XMVectorSet(jScale["x"], jScale["y"], jScale["z"], 0.0f);
	XMVECTOR translation = XMVectorSet(jTranslation["x"], jTranslation["y"], jTranslation["z"], 1.0f);

	//get base rotation vectors from json
	float basePitch = DirectX::XMConvertToRadians(jOrientation["pitch"]);
	float baseYaw = DirectX::XMConvertToRadians(jOrientation["yaw"]);
	float baseRoll = DirectX::XMConvertToRadians(jOrientation["roll"]);

	// handle special behaviors
	if (!jBehavior.empty()) {
		if (!jBehavior["rotateContinuous"].empty()) {
			json jRotateContinuous = jBehavior.value("rotateContinuous", nlohmann::json::object());
			// get the speed of rotation
			float rotationSpeedPitch = DirectX::XMConvertToRadians(jRotateContinuous.value("rotationSpeedPitch", 0.0f));
			float rotationSpeedYaw = DirectX::XMConvertToRadians(jRotateContinuous.value("rotationSpeedYaw", 0.0f));
			float rotationSpeedRoll = DirectX::XMConvertToRadians(jRotateContinuous.value("rotationSpeedRoll", 0.0f));

			currentNode->ContinuousRotation = 1; //set flag on node
			XMVECTOR ContinuousRotationVector = XMVectorSet(rotationSpeedPitch, rotationSpeedYaw, rotationSpeedRoll, 0.0f);
			XMStoreFloat4(&currentNode->ContinuousRotationVector, ContinuousRotationVector);
		}
	}

	XMMATRIX OrientationMatrix = XMMatrixRotationRollPitchYaw(basePitch, baseYaw, baseRoll);

	XMMATRIX LocalMatrix =
		XMMatrixScalingFromVector(scale) *
		OrientationMatrix *
		XMMatrixTranslationFromVector(translation);

	XMStoreFloat4x4(&currentNode->LocalMatrix, LocalMatrix);
}

void Scene::LoadSceneFromJson(std::string ScenePath,
	ResourceManager* rm, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) {
	std::ifstream input_json(ScenePath);

	json j;
	input_json >> j;

	//scene root node is created here
	SceneRoot = std::make_unique<Node>(rm, "SceneRoot");

	//the top level elements in the json are the top level nodes directly attached to the sceneroot
	if (j.is_array()) {
		for (const auto& jNode : j) {
			ParseNodeRecursive(jNode, SceneRoot.get(), rm, device, context);
		}
	}
};

