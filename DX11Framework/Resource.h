#pragma once

#include <wrl.h>
#include <filesystem>
#include "Structures.h"
#include "OBJLoader.h"
#include "DDSTextureLoader/DDSTextureLoader.h"

using Microsoft::WRL::ComPtr;

//Empty class used as base
class Resource {
public:
	Resource();
};

class MeshResource : Resource {
public:
	std::string MeshName;
	MeshData Mesh;

	MeshResource(std::string meshName, const std::filesystem::path& filepath, ComPtr<ID3D11Device>& device);
};

class TextureResource : Resource {
public:
	std::string TextureName;
	ComPtr<ID3D11ShaderResourceView> Texture;

	TextureResource(std::string textureName, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::filesystem::path& filepath);
};