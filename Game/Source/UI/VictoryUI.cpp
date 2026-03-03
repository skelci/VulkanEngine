#include "VictoryUI.hpp"

#include "GameInstance.hpp"
#include "Maps/GameMap.hpp"
#include "Maps/MenuMap.hpp"
#include "UI/LoadingScreen.hpp"

#include "Widgets/Button.hpp"
#include "Widgets/Image.hpp"
#include "Widgets/Text.hpp"
#include "Widgets/VerticalBox.hpp"


WVictoryUI::WVictoryUI() {
    // Semi-transparent overlay
    WImage* Bg = AddChild<WImage>();
    Bg->SetMaterialProperty("Color", SVector4(0.0f, 0.05f, 0.0f, 0.75f));
    GetSlotForChild(Bg)->Alignment = SVector4(0, 1, 0, 1);

    // Center panel
    WOverlay* Panel = AddChild<WOverlay>();
    GetSlotForChild(Panel)->Alignment = SVector4(0.5f);

    WImage* PanelBg = Panel->AddChild<WImage>();
    PanelBg->Size = SVector2(420, 350);
    PanelBg->SetMaterialProperty("Color", SVector4(0.1f, 0.15f, 0.1f));
    PanelBg->SetMaterialProperty("CornerRadius", SVector4(16));
    Panel->GetSlotForChild(PanelBg)->Alignment = SVector4(0, 1, 0, 1);

    WVerticalBox* VBox = Panel->AddChild<WVerticalBox>();
    Panel->GetSlotForChild(VBox)->Alignment = SVector4(0, 1, 0, 1);
    Panel->GetSlotForChild(VBox)->Padding = SVector4(30);

    CGameInstance* GI = GEngine->GetGameInstance<CGameInstance>();
    CGameMap* GameMap = Cast<CGameMap>(GEngine->GetWorld());
    int32 LevelScore = GameMap ? GameMap->GetScore() : 0;

    // Title
    WText* Title = VBox->AddChild<WText>();
    Title->SetText("Level " + std::to_string(GI->Level) + " Complete!");
    Title->FontSize = 36.0f;
    Title->SetMaterialProperty("Color", SVector4(1.0f, 0.85f, 0.2f));
    VBox->GetSlotForChild(Title)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(Title)->Padding = SVector4(0, 0, 0, 10);

    // Score
    WText* ScoreText = VBox->AddChild<WText>();
    ScoreText->SetText("Score: " + std::to_string(LevelScore));
    ScoreText->FontSize = 28.0f;
    ScoreText->SetMaterialProperty("Color", SVector4(0.9f, 0.9f, 0.9f));
    VBox->GetSlotForChild(ScoreText)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(ScoreText)->Padding = SVector4(0, 0, 0, 5);

    // Total score
    WText* TotalText = VBox->AddChild<WText>();
    TotalText->SetText("Total: " + std::to_string(GI->TotalScore + LevelScore));
    TotalText->FontSize = 22.0f;
    TotalText->SetMaterialProperty("Color", SVector4(0.7f, 0.7f, 0.7f));
    VBox->GetSlotForChild(TotalText)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(TotalText)->Padding = SVector4(0, 0, 0, 20);

    // Next Level button
    WButton* NextBtn = VBox->AddChild<WButton>();
    NextBtn->Size = SVector2(250, 50);
    NextBtn->SetText("Next Level");
    NextBtn->SetFontSize(24.0f);
    NextBtn->SetTextMaterialProperty("Color", SVector4(1));
    NextBtn->GetImageWidget()->SetMaterialProperty("Color", SVector4(0.15f, 0.55f, 0.25f));
    NextBtn->GetImageWidget()->SetMaterialProperty("CornerRadius", SVector4(8));
    VBox->GetSlotForChild(NextBtn)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(NextBtn)->Padding = SVector4(0, 5, 0, 5);
    NextBtn->OnClickedDelegate.Bind([LevelScore]() {
        CGameInstance* GI = GEngine->GetGameInstance<CGameInstance>();
        GI->TotalScore += LevelScore;
        GI->Level++;
        GI->UpdateLeaderboard();
        GEngine->GetRenderer()->ClearUIWidgets();
        GEngine->GetRenderer()->AddUIWidget<WLoadingScreen>();
        GEngine->OpenWorld(CGameMap::StaticClass());
    });

    // Main Menu button
    WButton* MenuBtn = VBox->AddChild<WButton>();
    MenuBtn->Size = SVector2(250, 50);
    MenuBtn->SetText("Main Menu");
    MenuBtn->SetFontSize(24.0f);
    MenuBtn->SetTextMaterialProperty("Color", SVector4(1));
    MenuBtn->GetImageWidget()->SetMaterialProperty("Color", SVector4(0.35f, 0.35f, 0.5f));
    MenuBtn->GetImageWidget()->SetMaterialProperty("CornerRadius", SVector4(8));
    VBox->GetSlotForChild(MenuBtn)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(MenuBtn)->Padding = SVector4(0, 5, 0, 5);
    MenuBtn->OnClickedDelegate.Bind([LevelScore]() {
        CGameInstance* GI = GEngine->GetGameInstance<CGameInstance>();
        GI->TotalScore += LevelScore;
        GI->Level++;
        GI->UpdateLeaderboard();
        GEngine->GetRenderer()->ClearUIWidgets();
        GEngine->GetRenderer()->AddUIWidget<WLoadingScreen>();
        GEngine->OpenWorld(CMenuMap::StaticClass());
    });
}
