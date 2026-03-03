#include "DefaultEngineConfig.hpp"

#include "GameInstance.hpp"
#include "Maps/GameMap.hpp"
#include "Maps/MenuMap.hpp"


void ConfigureEngine() {
    GEngineConfig.WorldClass = CMenuMap::StaticClass();
    GEngineConfig.GameInstanceClass = CGameInstance::StaticClass();
}
