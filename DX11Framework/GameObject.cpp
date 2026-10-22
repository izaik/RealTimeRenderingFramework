#include "GameObject.h"

GameObject::GameObject(DirectX::XMFLOAT4X4 world) {
	_World = world;
}

void GameObject::Draw(ComPtr<ID3D11DeviceContext> context) {
	context->IASetVertexBuffers(0, 1, _Mesh.VertexBuffer.GetAddressOf(), &_Mesh.VBStride, &_Mesh.VBOffset);
	context->IASetIndexBuffer(_Mesh.IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->DrawIndexed(_Mesh.IndexCount, 0, 0);
}

void GameObject::Update(float deltaTime) {
	XMStoreFloat4x4(&_World, XMMatrixIdentity() * XMMatrixRotationY(deltaTime * 0.17f));
}