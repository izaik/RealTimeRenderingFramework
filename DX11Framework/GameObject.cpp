#include "GameObject.h"

XMMATRIX GameObject::GetLocalObjectMatrix() {
	const XMMATRIX TransformX = XMMatrixRotationX(transform.EulerRot.x);
	const XMMATRIX TransformY = XMMatrixRotationY(transform.EulerRot.y);
	const XMMATRIX TransformZ = XMMatrixRotationZ(transform.EulerRot.z);

	const XMMATRIX RotationMatrix = TransformY * TransformX * TransformZ;

	return XMMatrixIdentity()
		* XMMatrixTranslation(transform.Position.x, transform.Position.y, transform.Position.z)
		* RotationMatrix
		* XMMatrixScaling(transform.Scale.x, transform.Scale.y, transform.Scale.z);
}

void GameObject::UpdateSelfAndChild() {
	if (Parent) {
		transform.Model = Parent->transform.Model * GetLocalObjectMatrix();
	}
	else {
		transform.Model = GetLocalObjectMatrix();
	}

	for (auto&& Child : Children) {
		Child->UpdateSelfAndChild();
	}
}

void Model::SetDiffuse(ComPtr<ID3D11ShaderResourceView>& in, ComPtr<ID3D11DeviceContext> context) {
	Diffuse = in;
	context->PSSetShaderResources(0, 1, in.GetAddressOf());
}

void Model::SetSpecular(ComPtr<ID3D11ShaderResourceView>& in, ComPtr<ID3D11DeviceContext> context) {
	Specular = in;
	context->PSSetShaderResources(1, 1, in.GetAddressOf());
}

void Model::Draw(
	ConstantBuffer cbData,
	ComPtr<ID3D11DeviceContext> context,
	ComPtr<ID3D11Buffer> constantBuffer,
	D3D11_MAPPED_SUBRESOURCE mappedSubresource)
{
	context->IASetVertexBuffers(0, 1, Mesh.VertexBuffer.GetAddressOf(), &Mesh.VBStride, &Mesh.VBOffset);
	context->IASetIndexBuffer(Mesh.IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	cbData.World = XMMatrixTranspose(this->transform.Model);
	context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, &cbData, sizeof(cbData));
	context->Unmap(constantBuffer.Get(), 0);

	context->DrawIndexed(Mesh.IndexCount, 0, 0);
}

void Model::Update(float deltaTime) {
	this->transform.EulerRot.y + 20 * deltaTime;
	this->UpdateSelfAndChild();
}
