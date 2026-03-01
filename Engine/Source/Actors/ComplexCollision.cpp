#include "ComplexCollision.hpp"

#include "MeshActor.hpp"


void AComplexCollision::SetCollisionMeshFromMesh(const CMesh& Mesh) {
    Vertices.clear();
    for (const SVertex& Vertex : Mesh.GetVertices()) {
        Vertices.emplace_back(Vertex.Position.x, Vertex.Position.y, Vertex.Position.z);
    }
    Indices = Mesh.GetIndices();
    IsCacheDirty = true;

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

const std::vector<SVector>& AComplexCollision::GetWorldVertices() const {
    const STransform& CurrentTransform = GetWorldTransform();

    // Check if transform has changed
    if (IsCacheDirty || CurrentTransform != CachedTransform) {
        CachedWorldVertices.resize(Vertices.size());
        for (size_t i = 0; i < Vertices.size(); ++i) {
            CachedWorldVertices[i] =
                (Vertices[i] * CurrentTransform.Scale).Rotated(CurrentTransform.Rotation) + CurrentTransform.Position;
        }
        CachedTransform = CurrentTransform;
        IsCacheDirty = false;
    }

    return CachedWorldVertices;
}
