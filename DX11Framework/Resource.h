#pragma once

#include <wrl.h>
#include <filesystem>
#include "Structures.h"
#include "OBJLoader.h"
#include "DDSTextureLoader/DDSTextureLoader.h"

using Microsoft::WRL::ComPtr;

class Resource {};

class MeshResource : Resource {
	MeshData Mesh;

public:
	MeshResource(const std::filesystem::path& filepath, ComPtr<ID3D11Device>& device);
};

class TextureResource : Resource {
	ComPtr<ID3D11ShaderResourceView> Texture;

public:
	TextureResource(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::filesystem::path& filepath);
};