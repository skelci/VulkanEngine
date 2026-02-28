#pragma once

#include "Actors/Actor.hpp"
#include "Object.hpp"

#include <memory>
#include <vector>

struct SHitResult;
class APhysicsBody;
class ABoxCollision;
class AComplexCollision;


class CWorld : public CObject {
    CLASS_BODY(CWorld, CObject)

protected:
    friend class CEngine;
    virtual void BeginPlay();

public:
    virtual void Tick(float DeltaTime);

    AActor* SpawnActor(const TSubclassOf<AActor>& ActorClass);

    template <typename T>
    T* SpawnActor(const TSubclassOf<AActor>& ActorClass) {
        return Cast<T>(SpawnActor(ActorClass));
    }

    void DestroyActor(AActor* Actor);

    const std::vector<std::unique_ptr<AActor>>& GetActors() const { return Actors; }

    template <typename T>
    std::vector<T*> GetActors() const;

    SVector Gravity = SVector(0, 0, -9.81f);

private:
    std::vector<std::unique_ptr<AActor>> Actors;

public:
    bool LineTrace(const SVector& Start, const SVector& End, SHitResult& OutHit) const;

protected:
    void SolveCollisions(float DeltaTime);
    bool AreBoundingBoxesOverlapping(
        const SVector& PosA, const SVector& ExtentA, const SVector& PosB, const SVector& ExtentB
    ) const;
    bool CheckCollision(const APhysicsBody* A, const APhysicsBody* B, SVector& OutPenetration) const;
    bool CheckCollisionBoxBox(const ABoxCollision* A, const ABoxCollision* B, SVector& OutPenetration) const;
    bool CheckCollisionBoxComplex(
        const ABoxCollision* Box, const AComplexCollision* Complex, SVector& OutPenetration
    ) const;
};


template <typename T>
inline std::vector<T*> CWorld::GetActors() const {
    std::vector<T*> Result;
    for (const auto& Actor : Actors) {
        if (T* Casted = Cast<T>(Actor.get())) {
            Result.push_back(Casted);
        }
    }
    return Result;
}
