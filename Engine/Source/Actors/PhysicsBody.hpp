#pragma once

#include "Actor.hpp"


class APhysicsBody;

struct SHitResult {
    APhysicsBody* OtherActor = nullptr;
    SVector Penetration = SVector(0);
    float DeltaTime = 0.0f;
    SVector Location = SVector(0);
    SVector Normal = SVector(0);
    float Distance = 0.0f;
};


class APhysicsBody : public AActor {
    CLASS_BODY(APhysicsBody, AActor)

public:
    virtual void Tick(float DeltaTime) override;

    virtual void OnHit(const SHitResult& HitResult, SVector& OutAdjustment);

    SVector Velocity;
    float Friction = 0.5f;
    bool SimulatePhysics = false;

protected:
    SVector CachedPosition;
};
