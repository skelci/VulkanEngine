#include "ComplexCollision.hpp"

#include "MeshActor.hpp"


void AComplexCollision::SetCollisionMeshFromMesh(const CMesh& Mesh) {
    Vertices.clear();
    for (const SVertex& Vertex : Mesh.GetVertices()) {
        Vertices.emplace_back(Vertex.Position.x, Vertex.Position.y, Vertex.Position.z);
    }
    Indices = Mesh.GetIndices();

    if (VisualMesh) {
        GetWorld()->DestroyActor(VisualMesh);
    }

    VisualMesh = SpawnChildActor<AMeshActor>(AMeshActor::StaticClass());

    std::vector<SVertex> VisualVertices;
    for (const SVector& Vertex : Vertices) {
        VisualVertices.push_back(
            {{Vertex.X, Vertex.Y, Vertex.Z}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
        );
    }
    VisualMesh->Model->Meshes.push_back(CMesh::CreateFromData(VisualVertices, Indices));
    VisualMesh->Model->SetMaterial(GetAsset<CMaterial>("Engine/Materials/Wireframe.mat"));

    VisualMesh->IsVisible = IsMeshVisible;
}

void AComplexCollision::SetVisibility(bool IsVisible) {
    IsMeshVisible = IsVisible;
    if (VisualMesh) VisualMesh->IsVisible = IsVisible;
}
