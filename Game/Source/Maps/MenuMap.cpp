#include "MenuMap.hpp"

#include "UI/MainMenu.hpp"


void CMenuMap::BeginPlay() {
    GInputManager->SetInputMode(true);
    GEngine->GetRenderer()->AddUIWidget<WMainMenu>();
}
