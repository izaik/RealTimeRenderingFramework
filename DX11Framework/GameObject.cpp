#include "GameObject.h"

GameObject::GameObject(DirectX::XMFLOAT4X4 world) {
	_World = world;
}

void GameObject::Draw(ComPtr<ID3D11DeviceContext> context) {
	_cbData.World = XMMatrixTranspose(XMLoadFloat4x4(&_World2));
	context->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
	context->Unmap(cBuffer.Get(), 0);
	context->IASetVertexBuffers(0, 1, _Mesh.VertexBuffer.GetAddressOf(), &_Mesh.VBStride, &_Mesh.VBOffset);
	context->IASetIndexBuffer(_Mesh.IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->DrawIndexed(_Mesh.IndexCount, 0, 0);
}

void GameObject::Update() {
	
}