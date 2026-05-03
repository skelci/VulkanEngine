#pragma once

#include "Core/Object.hpp"
#include "World.hpp"


struct SEngineConfig {
    TSubclassOf<CWorld> WorldClass = CWorld::StaticClass();
    TSubclassOf<CObject> GameInstanceClass = CObject::StaticClass();
    bool CompileShadersOnStartup = true;
    bool AllowDedicatedGPU = true;
};

extern SEngineConfig GEngineConfig;

void ConfigureEngine();
