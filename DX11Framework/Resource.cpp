#include "Resource.h"

MeshResource::MeshResource(const std::filesystem::path& filepath, ComPtr<ID3D11Device>& device) {
	Mesh = OBJLoader::Load(filepath.string().c_str(), device);
}

TextureResource::TextureResource(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::filesystem::path& filepath) {
    CreateDDSTextureFromFile(
        device.Get(),
        context.Get(),
        filepath.c_str(),
        nullptr,
        Texture.GetAddressOf()
    );
}