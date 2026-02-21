#include "DefaultEngineConfig.hpp"

#include "Maps/MenuMap.hpp"


void ConfigureEngine() { GEngineConfig.WorldClass = CMenuMap::StaticClass(); }
