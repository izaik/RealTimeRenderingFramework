#include "Node.h"

Node::Node(ResourceManager* rm, std::string name) {
	RM = rm;
	Name = name;
	XMStoreFloat4x4(&LocalMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
	Parent = nullptr;
}

void Node::UpdateWorldMatrix(float simpleCount) {
	XMMATRIX localMat = XMLoadFloat4x4(&LocalMatrix);

	if (Parent != nullptr) {
		XMMATRIX parentMat = XMLoadFloat4x4(&this->Parent->WorldMatrix);

		if (this->ContinuousRotation) {
			XMMATRIX rotMatrix = XMLoadFloat4x4(&this->ContinuousRotationMatrix);
			localMat *= (XMMatrixRotationRollPitchYaw()); //TODO: get roll pitch and yaw amounts from json (fed to node?)
		}

		XMStoreFloat4x4(&WorldMatrix, localMat * parentMat);

		
	}
	else {
		//if no parent exists use object localmatrix (should only be scene root node)
		WorldMatrix = LocalMatrix;
	}
	//propagate down to all child and sub child nodes
	for (auto& Child: Children) {
		if (Child != nullptr) {
			Child->UpdateWorldMatrix(simpleCount);
		}
	}
}

void Node::Draw(RenderContext& _RenderContext) {
	for (auto& Child : Children) {
		Child->Draw(_RenderContext);
	}
}

GeometryNode::GeometryNode(ResourceManager* RM, std::string name,
	const char* diffpath, const char* specpath, const char* meshpath,
	ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) : Node(RM, name)
{
	Diffuse = RM->GetResource<TextureResource>(diffpath, device, context, diffpath);
	Specular = RM->GetResource<TextureResource>(specpath, device, context, specpath);
	MeshRes = RM->GetResource<MeshResource>(meshpath, meshpath, device);
}

void GeometryNode::Draw(RenderContext& _RenderContext)
{
	//set textures
	_RenderContext.context->PSSetShaderResources(0, 1, Diffuse->Texture.GetAddressOf());
	_RenderContext.context->PSSetShaderResources(1, 1, Specular->Texture.GetAddressOf());

	//configure vertex and index buffers
	_RenderContext.context->IASetVertexBuffers(0, 1, MeshRes->Mesh.VertexBuffer.GetAddressOf(), &MeshRes->Mesh.VBStride, &MeshRes->Mesh.VBOffset);
	_RenderContext.context->IASetIndexBuffer(MeshRes->Mesh.IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

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
	_RenderContext.context->DrawIndexed(MeshRes->Mesh.IndexCount, 0, 0);

	//Call logic from parent class, child nodes may be of different type
	Node::Draw(_RenderContext);
}
