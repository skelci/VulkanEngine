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
    UpdateCache();
    return CachedWorldVertices;
}

SVector AComplexCollision::GetAABBMin() const {
    UpdateCache();
    return CachedAABBMin;
}

SVector AComplexCollision::GetAABBMax() const {
    UpdateCache();
    return CachedAABBMax;
}

void AComplexCollision::UpdateCache() const {
    const STransform& CurrentTransform = GetWorldTransform();
    if (IsCacheDirty || CurrentTransform != CachedTransform) {
        CachedAABBMin = SVector(DOUBLE_INF);
        CachedAABBMax = SVector(-DOUBLE_INF);
        CachedWorldVertices.resize(Vertices.size());

        for (size_t i = 0; i < Vertices.size(); ++i) {
            CachedWorldVertices[i] =
                (Vertices[i] * CurrentTransform.Scale).Rotated(CurrentTransform.Rotation) + CurrentTransform.Position;

            CachedAABBMin.X = std::min(CachedAABBMin.X, CachedWorldVertices[i].X);
            CachedAABBMin.Y = std::min(CachedAABBMin.Y, CachedWorldVertices[i].Y);
            CachedAABBMin.Z = std::min(CachedAABBMin.Z, CachedWorldVertices[i].Z);

            CachedAABBMax.X = std::max(CachedAABBMax.X, CachedWorldVertices[i].X);
            CachedAABBMax.Y = std::max(CachedAABBMax.Y, CachedWorldVertices[i].Y);
            CachedAABBMax.Z = std::max(CachedAABBMax.Z, CachedWorldVertices[i].Z);
        }

        CachedTransform = CurrentTransform;
        IsCacheDirty = false;
    }
}
