#include "Camera.h"

Camera::Camera(XMFLOAT3 position, XMFLOAT3 direction, XMFLOAT3 rotation, float aspect, float fov, float nearPlane, float farPlane) {
	Eye = position;
	To = direction;
	Up = rotation;

	AspectRatio = aspect;
	FOV = fov;
	NearDepth = nearPlane;
	FarDepth = farPlane;
};

void Camera::Update() {
	// if the camera moves update view matrix
	if (IsDirty) {
		XMStoreFloat4x4(&View, XMMatrixLookToLH(XMLoadFloat3(&Eye), XMLoadFloat3(&To), XMLoadFloat3(&Up)));
	}
};