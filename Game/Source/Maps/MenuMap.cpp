#include "MenuMap.hpp"

#include "GameInstance.hpp"
#include "UI/MainMenu.hpp"


void CMenuMap::BeginPlay() {
    GInputManager->SetInputMode(true);
    GEngine->GetRenderer()->ClearUIWidgets();
    GEngine->GetGameInstance<CGameInstance>()->LoadLeaderboard();
    GEngine->GetRenderer()->AddUIWidget<WMainMenu>();
}
