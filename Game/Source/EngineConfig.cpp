#include "DefaultEngineConfig.hpp"

#include "TestWorld.hpp"


void ConfigureEngine() { GEngineConfig.WorldClass = CTestWorld::StaticClass(); }
