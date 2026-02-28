#pragma once

#include "Object.hpp"
#include "Rotator.hpp"
#include "Vector.hpp"

class CWorld;


struct STransform {
    SVector Position = SVector(0);
    SRotator Rotation = SRotator(0);
    SVector Scale = SVector(1);
};


class AActor : public CObject {
    CLASS_BODY(AActor, CObject)

public:
    virtual void Tick(float DeltaTime) {}

    STransform Transform;
    STransform GetWorldTransform() const;

    AActor* Parent = nullptr;
    AActor* SpawnChildActor(const TSubclassOf<AActor>& ActorClass);

    template <typename T>
    T* SpawnChildActor(const TSubclassOf<AActor>& ActorClass) {
        return Cast<T>(SpawnChildActor(ActorClass));
    }

    CWorld* GetWorld() const;

private:
    friend CWorld;
    bool IsPendingDestroy = false;
};
