#pragma once

#include "CoreMinimal.hpp"
#include "World.hpp"


class CTestWorld : public CWorld {
    CLASS_BODY(CTestWorld, CWorld)

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    class ACamera* Camera;

    SVector InputVector = SVector(0);
    float FlySpeed = 1;

    void OnEscapePressed();
    void Move(EKeys Key);
    void LookAround(SVector2 Delta);
    void ScaleFlySpeed(float Amount);

    class AMeshActor* Character;
};
