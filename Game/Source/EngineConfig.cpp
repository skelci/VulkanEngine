#include "DefaultEngineConfig.hpp"

#include "Maps/GameMap.hpp"
#include "Maps/MenuMap.hpp"


void ConfigureEngine() { GEngineConfig.WorldClass = CGameMap::StaticClass(); }
