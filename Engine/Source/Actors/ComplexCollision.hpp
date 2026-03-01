#pragma once

#include "DataTypes.hpp"
#include "PhysicsBody.hpp"

#include <unordered_map>
#include <vector>

class CMesh;


struct SGridCell {
    int32 X, Y, Z;
    bool operator==(const SGridCell& Other) const { return X == Other.X && Y == Other.Y && Z == Other.Z; }
};

struct SGridCellHash {
    std::size_t operator()(const SGridCell& Cell) const {
        return (std::hash<int32>()(Cell.X) * 73856093) ^ (std::hash<int32>()(Cell.Y) * 19349663) ^
               (std::hash<int32>()(Cell.Z) * 83492791);
    }
};

class AComplexCollision : public APhysicsBody {
    CLASS_BODY(AComplexCollision, APhysicsBody)

public:
    void SetCollisionMeshFromMesh(const CMesh& Mesh);
    void SetVisibility(bool IsVisible);

    const std::vector<SVector>& GetWorldVertices() const;
    const std::vector<uint32>& GetIndices() const { return Indices; }

    SVector GetAABBMin() const;
    SVector GetAABBMax() const;

    std::vector<uint32> GetOverlappingTriangles(const SVector& BoundsMin, const SVector& BoundsMax) const;

protected:
    std::vector<SVector> Vertices;
    std::vector<uint32> Indices;

    class AMeshActor* VisualMesh = nullptr;
    bool IsMeshVisible = false;

    void UpdateCache() const;

    mutable SVector CachedAABBMin;
    mutable SVector CachedAABBMax;
    mutable STransform CachedTransform;
    mutable std::vector<SVector> CachedWorldVertices;
    mutable bool IsCacheDirty = true;

    float GridCellSize = 5.0f;
    mutable std::unordered_map<SGridCell, std::vector<uint32>, SGridCellHash> SpatialGrid;
};
