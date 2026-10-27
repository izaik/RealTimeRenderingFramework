#include "GameObject.h"

GameObject::GameObject(DirectX::XMFLOAT4X4 world) {
	World = world;
}

void GameObject::SetDiffuse(ComPtr<ID3D11ShaderResourceView>& in, ComPtr<ID3D11DeviceContext> context) {
	Diffuse = in;
	context->PSSetShaderResources(0, 1, in.GetAddressOf());
}

void GameObject::SetSpecular(ComPtr<ID3D11ShaderResourceView>& in, ComPtr<ID3D11DeviceContext> context) {
	Specular = in;
	context->PSSetShaderResources(1, 1, in.GetAddressOf());
}

void GameObject::Draw(
	ConstantBuffer cbData,
	ComPtr<ID3D11DeviceContext> context, 
	ComPtr<ID3D11Buffer> constantBuffer, 
	D3D11_MAPPED_SUBRESOURCE mappedSubresource) 
{
	context->IASetVertexBuffers(0, 1, Mesh.VertexBuffer.GetAddressOf(), &Mesh.VBStride, &Mesh.VBOffset);
	context->IASetIndexBuffer(Mesh.IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	cbData.World = XMMatrixTranspose(XMLoadFloat4x4(&World));
	context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, &cbData, sizeof(cbData));
	context->Unmap(constantBuffer.Get(), 0);

	context->DrawIndexed(Mesh.IndexCount, 0, 0);
}

void GameObject::Update(float deltaTime) {
	XMStoreFloat4x4(&World, XMMatrixIdentity() * XMMatrixRotationY(deltaTime * 0.17f) * XMMatrixScaling(0.6f, 0.6f, 0.6f));
}