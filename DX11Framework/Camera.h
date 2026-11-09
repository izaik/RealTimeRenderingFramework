#pragma once

#include <d3d11_4.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "Structures.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

enum MOVEMENT_DIRECTION {
	FORWARD,
	RIGHT,
	BACKWARD,
	LEFT,
};

//Camera
class Camera {
	XMFLOAT3 Position;
	XMFLOAT3 Front;
	XMFLOAT3 WorldUp;
	
	XMFLOAT3 Right;
	XMFLOAT3 Up;

	float Yaw;
	float Pitch;

	float AspectRatio;
	float FOV;
	float NearDepth;
	float FarDepth;
	float MovementSpeed;
	float CameraSensitivity;

	XMFLOAT4X4 Perspective;
	XMFLOAT4X4 View;

	bool NeedUpdate = true;

public:
	Camera(
		float aspect,
		XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 3.0f), 
		XMFLOAT3 direction = XMFLOAT3(0.0f, 0.0f, 1.0f), 
		XMFLOAT3 worldUp = XMFLOAT3(0.0f, 1.0f, 0.0f), 
		float fov = 75.0f, 
		float nearPlane = 0.01f, 
		float farPlane = 100.0f, 
		float movementSpeed = 2.5f,
		float yaw = -90.0f, 
		float pitch = 0.0f,
		float cameraSensitivity = 0.2f);
	virtual ~Camera() = default;
	void ProcessKeyboardInput(MOVEMENT_DIRECTION movementDirection, float deltaTime);
	void ProcessMouseInput(float xOffset, float yOffset, float deltaTime);
	void Update();

	XMFLOAT3 GetPosition() { return Position; }
	XMFLOAT4X4 GetPerspective() { return Perspective; }
	XMFLOAT4X4 GetView() { return View; }
};