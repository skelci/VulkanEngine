#include "Widget.hpp"

#include "Assets/Material.hpp"
#include "Assets/Mesh.hpp"
#include "EngineStatics.hpp"


WWidget::WWidget() {
    Mesh = GetAsset<CMesh>("Engine/Meshes/Quad.obj");
    Mesh->Material = GetAsset<CMaterial>("Engine/Materials/Widget.mat");
}

void WWidget::SetMaterial(std::shared_ptr<CMaterial> Material) { Mesh->Material = Material; }
