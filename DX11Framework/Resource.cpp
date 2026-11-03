#include "Resource.h"

Resource::Resource() {};

MeshResource::MeshResource(std::string meshName, const std::filesystem::path& filepath, ComPtr<ID3D11Device>& device) {
    MeshName = meshName;
    Mesh = OBJLoader::Load(filepath.string().c_str(), device);
}

TextureResource::TextureResource(std::string textureName, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::filesystem::path& filepath) {
    TextureName = textureName;
    CreateDDSTextureFromFile(
        device.Get(),
        context.Get(),
        filepath.c_str(),
        nullptr,
        Texture.GetAddressOf()
    );
}