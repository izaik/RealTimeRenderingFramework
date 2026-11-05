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

class MeshResource : public Resource {
public:
	MeshData Mesh;

	MeshResource(const std::filesystem::path& filepath, ComPtr<ID3D11Device>& device);
};

class TextureResource : public Resource {
public:
	ComPtr<ID3D11ShaderResourceView> Texture;

	TextureResource(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::filesystem::path& filepath);
};