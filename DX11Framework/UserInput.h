#pragma once

#include <windows.h>
#include <array>
#include <wrl.h>
#include <d3d11_4.h>
#include "ScanCode.h"
#include "Camera.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class UserInput {
	std::array<bool, SCANCODE_COUNT> _keys;
	
	float mouseXOffset = 0.0f;
	float mouseYOffset = 0.0f;
public:
	UserInput();
	LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void OnKeyDown(LPARAM lParam);
	void OnKeyUp(LPARAM lParam);
	void ProcessInput(ComPtr<ID3D11DeviceContext>& context, 
		ComPtr<ID3D11RasterizerState>& wireframeState, ComPtr<ID3D11RasterizerState>& fillState,
		Camera* camera, float deltaTime);
};