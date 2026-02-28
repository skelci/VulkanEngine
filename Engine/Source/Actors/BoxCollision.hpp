#pragma once

#include "PhysicsBody.hpp"

class AMeshActor;


class ABoxCollision : public APhysicsBody {
    CLASS_BODY(ABoxCollision, APhysicsBody)

public:
    ABoxCollision();

    SVector GetBoxExtent() const { return BoxExtent; }
    void SetBoxExtent(const SVector& NewExtent);

    void SetVisibility(bool IsVisible);

protected:
    SVector BoxExtent = SVector(0.5f);
    AMeshActor* VisualMesh;
};
