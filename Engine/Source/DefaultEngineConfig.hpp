#pragma once

#include "Core/Object.hpp"
#include "World.hpp"


struct SEngineConfig {
    TSubclassOf<CWorld> WorldClass = CWorld::StaticClass();
};

extern SEngineConfig GEngineConfig;

void ConfigureEngine();
