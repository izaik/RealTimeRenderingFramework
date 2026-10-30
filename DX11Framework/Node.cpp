#include "Node.h"

Node::Node(std::string name) {
	Name = name;
	LocalMatrix = XMMatrixIdentity();
	WorldMatrix = XMMatrixIdentity();
	Parent = nullptr;
}

void Node::UpdateWorldMatrix() {
	if (Parent) {
		WorldMatrix = LocalMatrix * this->Parent->WorldMatrix;
	}
	else {
		//if no parent exists use object localmatrix (should only be scene root node)
		WorldMatrix = LocalMatrix;
	}

	for (auto& Child: Children) {
		Child->UpdateWorldMatrix();
	}
}


void GeometryNode::SetDiffuse(ComPtr<ID3D11ShaderResourceView>& in, ComPtr<ID3D11DeviceContext> context) {
	Diffuse = in;
	context->PSSetShaderResources(0, 1, in.GetAddressOf());
}

void GeometryNode::SetSpecular(ComPtr<ID3D11ShaderResourceView>& in, ComPtr<ID3D11DeviceContext> context) {
	Specular = in;
	context->PSSetShaderResources(1, 1, in.GetAddressOf());
}

void GeometryNode::Draw(
	ConstantBuffer cbData,
	ComPtr<ID3D11DeviceContext> context,
	ComPtr<ID3D11Buffer> constantBuffer,
	D3D11_MAPPED_SUBRESOURCE mappedSubresource)
{
	context->IASetVertexBuffers(0, 1, Mesh.VertexBuffer.GetAddressOf(), &Mesh.VBStride, &Mesh.VBOffset);
	context->IASetIndexBuffer(Mesh.IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	cbData.World = XMMatrixTranspose(LocalMatrix);
	context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, &cbData, sizeof(cbData));
	context->Unmap(constantBuffer.Get(), 0);

	context->DrawIndexed(Mesh.IndexCount, 0, 0);
}

void GeometryNode::Update(float deltaTime) {
	WorldMatrix = XMMatrixIdentity() * XMMatrixRotationY(deltaTime * 0.17f) * XMMatrixScaling(0.6f, 0.6f, 0.6f);
}
