#include "DefaultEngineConfig.hpp"

#include "MenuMap.hpp"


void ConfigureEngine() { GEngineConfig.WorldClass = CMenuMap::StaticClass(); }
