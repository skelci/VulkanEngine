#include "Image.hpp"

#include "Assets/Material.hpp"
#include "Assets/Mesh.hpp"
#include "EngineStatics.hpp"


WImage::WImage() {
    Mesh = GetAsset<CMesh>("Engine/Meshes/Quad.obj");
    Mesh->Material = GetAsset<CMaterial>("Engine/Materials/Widget.mat");
}

void WImage::SetMaterial(std::shared_ptr<CMaterial> Material) { Mesh->Material = Material; }
