#pragma once

#include "CoreMinimal.hpp"


class CGameMap : public CWorld {
    CLASS_BODY(CGameMap, CWorld)

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    class ACamera* Camera;

    SVector InputVector;
    float FlySpeed = 1;

    void OnEscapePressed();
    void Move(EKeys Key);
    void LookAround(SVector2 Delta);
    void ScaleFlySpeed(float Amount);

    class AMeshActor* Character;
};
