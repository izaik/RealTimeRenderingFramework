#pragma once

#include <DirectXMath.h>
#include <wrl.h>
#include <string>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct SimpleVertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;

	bool operator<(const SimpleVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	};
};