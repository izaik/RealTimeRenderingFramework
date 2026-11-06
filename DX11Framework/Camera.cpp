#include "Camera.h"

Camera::Camera() {
	Eye = XMFLOAT3(0, 0, 0);
	At = XMFLOAT3(0, 0, 0);
	Up = XMFLOAT3(0, 0, 0);
};

void Camera::Update(XMFLOAT4X4& viewMatrix) {
	XMStoreFloat4x4(&viewMatrix, XMMatrixLookToLH(XMLoadFloat3(&Eye), XMLoadFloat3(&At), XMLoadFloat3(&Up)));
};