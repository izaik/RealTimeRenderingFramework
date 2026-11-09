#include "Camera.h"

Camera::Camera(
	float aspect, 
	XMFLOAT3 position, 
	XMFLOAT3 direction, 
	XMFLOAT3 worldUp,
	float fov, 
	float nearPlane, 
	float farPlane, 
	float movementSpeed,
	float yaw, 
	float pitch,
	float cameraSensitivity) 
{
	Position = position;
	Front = direction;
	WorldUp = worldUp;

	AspectRatio = aspect;
	FOV = fov;
	NearDepth = nearPlane;
	FarDepth = farPlane;
	MovementSpeed = movementSpeed;
	CameraSensitivity = cameraSensitivity;

	Yaw = yaw;
	Pitch = pitch;

	XMStoreFloat4x4(&Perspective, XMMatrixPerspectiveFovLH(XMConvertToRadians(FOV), aspect, 0.01f, 100.0f)); //create perspective matrix
	this->Update(); //create view matrix and calc front, right, up vectors
};

void Camera::ProcessKeyboardInput(MOVEMENT_DIRECTION movementDirection, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;

	// if there is a change set flag to dirty and update camera will be called next frame
	if (movementDirection == FORWARD) {
		XMVECTOR front = XMLoadFloat3(&Front);
		XMVECTOR pos = XMLoadFloat3(&Position);
		pos += front * velocity;
		XMStoreFloat3(&Position, pos);
		NeedUpdate = true;
	}
	if (movementDirection == RIGHT) {
		XMVECTOR right = XMLoadFloat3(&Right);
		XMVECTOR pos = XMLoadFloat3(&Position);
		pos -= right * velocity;
		XMStoreFloat3(&Position, pos);
		NeedUpdate = true;
	}
	if (movementDirection == BACKWARD) {
		XMVECTOR front = XMLoadFloat3(&Front);
		XMVECTOR pos = XMLoadFloat3(&Position);
		pos -= front * velocity;
		XMStoreFloat3(&Position, pos);
		NeedUpdate = true;
	}
	if (movementDirection == LEFT) {
		XMVECTOR right = XMLoadFloat3(&Right);
		XMVECTOR pos = XMLoadFloat3(&Position);
		pos += right * velocity;
		XMStoreFloat3(&Position, pos);
		NeedUpdate = true;
	}
};

void Camera::ProcessMouseInput(float xOffset, float yOffset, float deltaTime) {
	Yaw += xOffset  * CameraSensitivity;
	Pitch += yOffset * CameraSensitivity;
	NeedUpdate = true;
	// TODO: Handle input from mouse
};

void Camera::Update() {
	// if the camera moves update view matrix
	if (NeedUpdate) {
		//clamp pitch to stop camera flipping
		if (Pitch > 89.0f)  Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;

		//recalc front vector
		Front.x = cos(XMConvertToRadians(Yaw)) * cos(XMConvertToRadians(Pitch));
		Front.y = sin(XMConvertToRadians(Pitch));
		Front.z = sin(XMConvertToRadians(Yaw)) * cos(XMConvertToRadians(Pitch));
		XMVECTOR f = XMLoadFloat3(&Front);
		f = XMVector3Normalize(f);
		XMStoreFloat3(&Front, f);

		//recalc right vector
		XMVECTOR wU = XMLoadFloat3(&WorldUp);
		XMVECTOR r = XMVector3Cross(wU, f); //cross world up with front because dx11 is lhanded
		r = XMVector3Normalize(r);
		XMStoreFloat3(&Right, r);

		//recalc up vector
		XMVECTOR u = XMVector3Cross(r, f);
		u = XMVector3Normalize(u);
		XMStoreFloat3(&Up, u);

		XMStoreFloat4x4(&View, XMMatrixLookToLH(XMLoadFloat3(&Position), f, u));
		NeedUpdate = false;
	}
};
