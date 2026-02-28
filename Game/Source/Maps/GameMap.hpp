#pragma once

#include "Actors/BoxCollision.hpp"
#include "CoreMinimal.hpp"


class CGameMap : public CWorld {
    CLASS_BODY(CGameMap, CWorld)

protected:
    virtual void BeginPlay() override;

public:
    void AddScore(int32 Amount = 1);
    void OnEscapePressed();
    void SwitchCharacterBoat();

    class AController* Controller;
    class ACharacter* Player;

private:
    bool IsInBoat = false;
    class ABoat* Boat;

    class WPlaytimeUI* PlaytimeUI;
    int32 Score = 0;

protected:
    ABoxCollision* SpawnAtRandomLocation(const TSubclassOf<ABoxCollision>& ActorClass);
};
