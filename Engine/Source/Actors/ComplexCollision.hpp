#pragma once

#include "DataTypes.hpp"
#include "PhysicsBody.hpp"

#include <vector>

class CMesh;


class AComplexCollision : public APhysicsBody {
    CLASS_BODY(AComplexCollision, APhysicsBody)

public:
    void SetCollisionMeshFromMesh(const CMesh& Mesh);
    void SetVisibility(bool IsVisible);

    const std::vector<SVector>& GetWorldVertices() const;
    const std::vector<uint32>& GetIndices() const { return Indices; }

    SVector GetAABBMin() const;
    SVector GetAABBMax() const;

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
};
