#pragma once

#include "Actor.hpp"


class APhysicsBody;

struct SHitResult {
    APhysicsBody* OtherActor;
    SVector Penetration;
    float DeltaTime;
};


class APhysicsBody : public AActor {
    CLASS_BODY(APhysicsBody, AActor)

public:
    virtual void Tick(float DeltaTime) override;

    virtual void OnHit(const SHitResult& HitResult, SVector& OutAdjustment);

    SVector Velocity;
    bool SimulatePhysics = false;

protected:
    SVector CachedPosition;
};
