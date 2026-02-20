#include "Image.hpp"

#include "Assets/Material.hpp"
#include "Assets/Mesh.hpp"
#include "EngineStatics.hpp"


WImage::WImage() {
    Mesh = GetAsset<CMesh>("Engine/Meshes/Quad.obj", true);
    Mesh->Material = GetAsset<CMaterial>("Engine/Materials/Image.mat", true);
}

void WImage::SetMaterial(std::shared_ptr<CMaterial> Material) { Mesh->Material = Material; }

CMaterial* WImage::GetMaterial() const { return Mesh->Material.get(); }
