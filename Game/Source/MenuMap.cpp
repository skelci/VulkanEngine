#include "MenuMap.hpp"

#include "UI/MainMenu.hpp"


void CMenuMap::BeginPlay() { GEngine->AddUIWidget<WMainMenu>(); }
