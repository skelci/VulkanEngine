#pragma once

#include "CoreMinimal.hpp"


class CGameMap : public CWorld {
    CLASS_BODY(CGameMap, CWorld)

protected:
    virtual void BeginPlay() override;

private:
    void OnEscapePressed();
    void SwitchCharacterBoat();

    bool IsInBoat = false;
    class AController* Controller;
    class ACharacter* Player;
    class ABoat* Boat;
};
