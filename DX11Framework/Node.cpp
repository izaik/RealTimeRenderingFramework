#include "Node.h"

Node::Node(std::string name) {
	Name = name;
	XMStoreFloat4x4(&LocalMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
	Parent = nullptr;
}

void Node::UpdateWorldMatrix() {
	XMMATRIX localMat = XMLoadFloat4x4(&LocalMatrix);

	if (Parent != nullptr) {
		XMMATRIX parentMat = XMLoadFloat4x4(&this->Parent->WorldMatrix);

		XMStoreFloat4x4(&WorldMatrix, localMat * parentMat);
	}
	else {
		//if no parent exists use object localmatrix (should only be scene root node)
		WorldMatrix = LocalMatrix;
	}
	//propagate down to all child and sub child nodes
	for (auto& Child: Children) {
		if (Child != nullptr) {
			Child->UpdateWorldMatrix();
		}
	}
}

void Node::Draw(RenderContext& _RenderContext) {
	for (auto& Child : Children) {
		Child->Draw(_RenderContext);
	}
}

GeometryNode::GeometryNode(std::string name, 
	TextureResource& Diffuse,
	TextureResource& Specular,
	const char* filepath, ComPtr<ID3D11Device>& device) : Node(name)
{
	Diffuse; //change once resource manager ready
	Specular; //change once resource manager ready
	Mesh = OBJLoader::Load(filepath, device);
}

void GeometryNode::Draw(RenderContext& _RenderContext)
{
	//set textures
	_RenderContext.context->PSSetShaderResources(0, 1, Diffuse.GetAddressOf());
	_RenderContext.context->PSSetShaderResources(1, 1, Specular.GetAddressOf());

	//configure vertex and index buffers
	_RenderContext.context->IASetVertexBuffers(0, 1, Mesh.VertexBuffer.GetAddressOf(), &Mesh.VBStride, &Mesh.VBOffset);
	_RenderContext.context->IASetIndexBuffer(Mesh.IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//send world matrix to constant buffer and use local variable to not overwrite shared data
	ConstantBuffer localCBufferData = *(_RenderContext.cbData);
	XMMATRIX FinalWorld = XMLoadFloat4x4(&WorldMatrix);
	localCBufferData.World = XMMatrixTranspose(FinalWorld);

	//Write constant buffer data onto GPU
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	_RenderContext.context->Map(_RenderContext.constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, &localCBufferData, sizeof(ConstantBuffer));
	_RenderContext.context->Unmap(_RenderContext.constantBuffer, 0);

	_RenderContext.context->VSSetConstantBuffers(0, 1, &_RenderContext.constantBuffer);
	_RenderContext.context->PSSetConstantBuffers(0, 1, &_RenderContext.constantBuffer);

	//draw object
	_RenderContext.context->DrawIndexed(Mesh.IndexCount, 0, 0);

	//Call logic from parent class, child nodes may be of different type
	Node::Draw(_RenderContext);
}
