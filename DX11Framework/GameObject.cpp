#include "GameObject.h"

class GameObject {

	void Update(float deltaTime)
	{

	}

	void Draw(ComPtr<ID3D11DeviceContext> context)
	{
		context->IASetVertexBuffers(0, 1, .VertexBuffer.GetAddressOf(), &_3dsMaxModel.VBStride, &_3dsMaxModel.VBOffset);
		context->IASetIndexBuffer(_3dsMaxModel.IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	}
};