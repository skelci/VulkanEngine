#include "ComplexCollision.hpp"

#include "MeshActor.hpp"

#include <unordered_set>


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

        SpatialGrid.clear();
        for (size_t i = 0; i < Indices.size(); i += 3) {
            const SVector& V0 = CachedWorldVertices[Indices[i]];
            const SVector& V1 = CachedWorldVertices[Indices[i + 1]];
            const SVector& V2 = CachedWorldVertices[Indices[i + 2]];

            SVector TriMin =
                SVector(std::min({V0.X, V1.X, V2.X}), std::min({V0.Y, V1.Y, V2.Y}), std::min({V0.Z, V1.Z, V2.Z}));
            SVector TriMax =
                SVector(std::max({V0.X, V1.X, V2.X}), std::max({V0.Y, V1.Y, V2.Y}), std::max({V0.Z, V1.Z, V2.Z}));

            int32 minX = static_cast<int32>(std::floor(TriMin.X / GridCellSize));
            int32 maxX = static_cast<int32>(std::floor(TriMax.X / GridCellSize));
            int32 minY = static_cast<int32>(std::floor(TriMin.Y / GridCellSize));
            int32 maxY = static_cast<int32>(std::floor(TriMax.Y / GridCellSize));
            int32 minZ = static_cast<int32>(std::floor(TriMin.Z / GridCellSize));
            int32 maxZ = static_cast<int32>(std::floor(TriMax.Z / GridCellSize));

            for (int32 x = minX; x <= maxX; ++x) {
                for (int32 y = minY; y <= maxY; ++y) {
                    for (int32 z = minZ; z <= maxZ; ++z) {
                        SpatialGrid[{x, y, z}].push_back(static_cast<uint32>(i));
                    }
                }
            }
        }

        CachedTransform = CurrentTransform;
        IsCacheDirty = false;
    }
}

std::vector<uint32> AComplexCollision::GetOverlappingTriangles(
    const SVector& BoundsMin, const SVector& BoundsMax
) const {
    UpdateCache();

    int32 minX = static_cast<int32>(std::floor(BoundsMin.X / GridCellSize));
    int32 maxX = static_cast<int32>(std::floor(BoundsMax.X / GridCellSize));
    int32 minY = static_cast<int32>(std::floor(BoundsMin.Y / GridCellSize));
    int32 maxY = static_cast<int32>(std::floor(BoundsMax.Y / GridCellSize));
    int32 minZ = static_cast<int32>(std::floor(BoundsMin.Z / GridCellSize));
    int32 maxZ = static_cast<int32>(std::floor(BoundsMax.Z / GridCellSize));

    std::unordered_set<uint32> UniqueTriangles;
    for (int32 x = minX; x <= maxX; ++x) {
        for (int32 y = minY; y <= maxY; ++y) {
            for (int32 z = minZ; z <= maxZ; ++z) {
                auto it = SpatialGrid.find({x, y, z});
                if (it != SpatialGrid.end()) {
                    for (uint32 TriIdx : it->second) {
                        UniqueTriangles.insert(TriIdx);
                    }
                }
            }
        }
    }

    return std::vector<uint32>(UniqueTriangles.begin(), UniqueTriangles.end());
}
