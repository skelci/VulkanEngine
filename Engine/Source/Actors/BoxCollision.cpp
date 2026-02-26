#include "BoxCollision.hpp"

#include "Actors/MeshActor.hpp"
#include "Assets/Model.hpp"


ABoxCollision::ABoxCollision() {
    VisualMesh = GetWorld()->SpawnActor<AMeshActor>(AMeshActor::StaticClass());
    VisualMesh->Parent = this;
    VisualMesh->Model = GetAsset<CModel>("Engine/Meshes/Cube.obj");
    VisualMesh->Model->SetMaterial(GetAsset<CMaterial>("Engine/Materials/Wireframe.mat"));

    SetVisibility(false);
}

void ABoxCollision::SetBoxExtent(const SVector& NewExtent) {
    BoxExtent = NewExtent;
    VisualMesh->Transform.Scale = BoxExtent * 2.0f;
}

void ABoxCollision::SetVisibility(bool IsVisible) { VisualMesh->IsVisible = IsVisible; }
