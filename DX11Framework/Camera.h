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
	XMFLOAT3 At;
	XMFLOAT3 Up;
	XMMATRIX Perspective;
	float aspect;

	Camera();
	void Update(XMFLOAT4X4& viewMatrix);
};



//XMStoreFloat4x4(&_View, XMMatrixLookAtLH(XMLoadFloat3(&Eye), XMLoadFloat3(&At), XMLoadFloat3(&Up)));

//Projection
//float aspect = _viewport.Width / _viewport.Height;
//XMMATRIX perspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(90), aspect, 0.01f, 100.0f);
//XMStoreFloat4x4(&_Projection, perspective);

//store camera position for phong
//_cbData.camPos = Eye;