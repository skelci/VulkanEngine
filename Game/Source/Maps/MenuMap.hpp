#pragma once

#include "CoreMinimal.hpp"
#include "World.hpp"


class CMenuMap : public CWorld {
    CLASS_BODY(CMenuMap, CWorld)

public:
    virtual void BeginPlay() override;
};
