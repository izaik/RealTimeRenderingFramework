#pragma once

#include <d3d11_4.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "Structures.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

//Camera
class Camera {
	XMFLOAT3 Eye;
	XMFLOAT3 To;
	XMFLOAT3 Up;

	float AspectRatio;
	float FOV;
	float NearDepth;
	float FarDepth;

	XMFLOAT4X4 Perspective;
	XMFLOAT4X4 View;

	bool IsDirty = 1;

public:
	Camera(XMFLOAT3 position, XMFLOAT3 direction, XMFLOAT3 rotation, float aspect, float fov, float nearPlane, float farPlane);
	void Update();

	XMFLOAT3 GetPosition() { return Eye; }
	XMFLOAT4X4 GetPerspective() { return Perspective; }
	XMFLOAT4X4 GetView() { return View; }
};



//XMStoreFloat4x4(&_View, XMMatrixLookAtLH(XMLoadFloat3(&Eye), XMLoadFloat3(&At), XMLoadFloat3(&Up)));

//Projection
//float aspect = _viewport.Width / _viewport.Height;
//XMMATRIX perspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(90), aspect, 0.01f, 100.0f);
//XMStoreFloat4x4(&_Projection, perspective);

//store camera position for phong
//_cbData.camPos = Eye;