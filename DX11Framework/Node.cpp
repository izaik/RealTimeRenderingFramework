#include "Node.h"

Node::Node(ResourceManager* rm, std::string name) {
	RM = rm;
	Name = name;
	XMStoreFloat4x4(&LocalMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&WorldInvTransposeMatrix, XMMatrixIdentity());
	XMStoreFloat4(&ContinuousRotationVector, XMVectorZero()); //initialise as empty
	Parent = nullptr;
}

void Node::UpdateWorldMatrix(float simpleCount) {
	XMMATRIX localMat = XMLoadFloat4x4(&LocalMatrix);

	if (Parent != nullptr) {
		XMMATRIX parentMat = XMLoadFloat4x4(&this->Parent->WorldMatrix);

		if (this->ContinuousRotation) {
			localMat *= (XMMatrixRotationRollPitchYaw(
				simpleCount * ContinuousRotationVector.x, 
				simpleCount * ContinuousRotationVector.y, 
				simpleCount * ContinuousRotationVector.z));
		}
		XMMATRIX finalMat = localMat * parentMat;
		XMMATRIX invTransposeMat = XMMatrixTranspose(XMMatrixInverse(nullptr, finalMat));
		XMStoreFloat4x4(&WorldMatrix, finalMat);
		XMStoreFloat4x4(&WorldInvTransposeMatrix, invTransposeMat);
	}
	else {
		//if no parent exists use object localmatrix (should only be scene root node)
		XMMATRIX invTransposeMat = XMMatrixTranspose(XMMatrixInverse(nullptr, localMat));
		WorldMatrix = LocalMatrix;
		XMStoreFloat4x4(&WorldInvTransposeMatrix, invTransposeMat);
		
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
	const char* diffpath, const char* specpath, const char* normpath, float shininess, const char* meshpath,
	ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) : Node(RM, name)
{
	Diffuse = RM->GetResource<TextureResource>(diffpath, device, context, diffpath);
	Specular = RM->GetResource<TextureResource>(specpath, device, context, specpath);
	Normal = RM->GetResource<TextureResource>(normpath, device, context, normpath);
	Shininess = shininess;
	MeshRes = RM->GetResource<MeshResource>(meshpath, meshpath, device);
}

void GeometryNode::Draw(RenderContext& _RenderContext)
{
	//set textures
	_RenderContext.context->PSSetShaderResources(0, 1, Diffuse->Texture.GetAddressOf());
	_RenderContext.context->PSSetShaderResources(1, 1, Specular->Texture.GetAddressOf());
	_RenderContext.context->PSSetShaderResources(2, 1, Normal->Texture.GetAddressOf());

	//configure vertex and index buffers
	_RenderContext.context->IASetVertexBuffers(0, 1, MeshRes->Mesh.VertexBuffer.GetAddressOf(), &MeshRes->Mesh.VBStride, &MeshRes->Mesh.VBOffset);
	_RenderContext.context->IASetIndexBuffer(MeshRes->Mesh.IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	
	//send world matrix (and transposed inverse) to constant buffer and use local variable to not overwrite shared data
	ConstantBuffer localCBufferData = *(_RenderContext.cbData);
	XMMATRIX FinalWorld = XMLoadFloat4x4(&WorldMatrix);
	localCBufferData.World = XMMatrixTranspose(FinalWorld);
	XMMATRIX FinalWorldInverseTranspose = XMLoadFloat4x4(&WorldInvTransposeMatrix);
	localCBufferData.WorldInverseTranspose = XMMatrixTranspose(FinalWorldInverseTranspose);

	//send shininess to constant buffer
	_RenderContext.cbData->Shininess = Shininess;

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

LightNode::LightNode(ResourceManager* RM, std::string name, XMFLOAT4 diffuse, XMFLOAT4 ambient, XMFLOAT4 specular, XMFLOAT3 direction) : Node(RM, name) {
	Diffuse = diffuse;
	Ambient = ambient;
	Specular = specular;
	Direction = direction;
};

void LightNode::Draw(RenderContext& _RenderContext) {
	_RenderContext.cbData->light.Diffuse = Diffuse;
	_RenderContext.cbData->light.Ambient = Ambient;
	_RenderContext.cbData->light.Specular = Specular;
	_RenderContext.cbData->light.Direction = Direction;
	//TODO add logic for different light types + multiple lights

	Node::Draw(_RenderContext);
}

SkyboxNode::SkyboxNode(ResourceManager* RM, std::string name, const char* skyboxpath,
	ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) : Node(RM, name) 
{
	Skybox = RM->GetResource<TextureResource>(skyboxpath, device, context, skyboxpath);
	std::string cubepath = "Models/Made In 3ds Max/cube.obj"; //making the assumption here that the skybox will always be a cube
	Cube = RM->GetResource<MeshResource>(cubepath, cubepath, device);
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&LocalMatrix, XMMatrixIdentity());
};

void SkyboxNode::Draw(RenderContext& _RenderContext) {
	//set rasterizer state to skybox
	_RenderContext.context->RSSetState(_RenderContext._skyboxState);
	//set depth stencil state to skybox
	_RenderContext.context->OMSetDepthStencilState(_RenderContext._depthStencilSkybox, 0);

	//draw skybox 
	//set shader resource to the texture cube
	_RenderContext.context->PSSetShaderResources(0, 1, Skybox->Texture.GetAddressOf());

	//configure vertex and index buffers
	_RenderContext.context->IASetVertexBuffers(0, 1, Cube->Mesh.VertexBuffer.GetAddressOf(), &Cube->Mesh.VBStride, &Cube->Mesh.VBOffset);
	_RenderContext.context->IASetIndexBuffer(Cube->Mesh.IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

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
	_RenderContext.context->DrawIndexed(Cube->Mesh.IndexCount, 0, 0);
	
	//revert settings/shaders/state to normal
	_RenderContext.context->RSSetState(_RenderContext._fillState);
	_RenderContext.context->OMSetDepthStencilState(_RenderContext._defaultDepthStencil, 0);
	//do not call draw for other nodes the skybox should have no child nodes
};

