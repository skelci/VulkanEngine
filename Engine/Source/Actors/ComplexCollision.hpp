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

    std::vector<SVector> Vertices;
    std::vector<uint32> Indices;

protected:
    class AMeshActor* VisualMesh = nullptr;
    bool IsMeshVisible = false;
};
