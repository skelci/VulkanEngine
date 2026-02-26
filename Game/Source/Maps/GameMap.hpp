#pragma once

#include "CoreMinimal.hpp"


class CGameMap : public CWorld {
    CLASS_BODY(CGameMap, CWorld)

protected:
    virtual void BeginPlay() override;

private:
    void OnEscapePressed();
};
