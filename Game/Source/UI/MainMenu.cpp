#include "MainMenu.hpp"

#include "GameInstance.hpp"
#include "Maps/GameMap.hpp"
#include "UI/LeaderboardUI.hpp"
#include "UI/LoadingScreen.hpp"

#include "Assets/Material.hpp"
#include "Widgets/Button.hpp"
#include "Widgets/Image.hpp"
#include "Widgets/InputBox.hpp"
#include "Widgets/Text.hpp"
#include "Widgets/VerticalBox.hpp"


WMainMenu::WMainMenu() {
    // Full-screen dark background
    WImage* Bg = AddChild<WImage>();
    Bg->SetMaterialProperty("Color", SVector4(0.08f, 0.05f, 0.15f));
    GetSlotForChild(Bg)->Alignment = SVector4(0, 1, 0, 1);

    // Center panel
    WOverlay* Panel = AddChild<WOverlay>();
    GetSlotForChild(Panel)->Alignment = SVector4(0.5f);

    WImage* PanelBg = Panel->AddChild<WImage>();
    PanelBg->Size = SVector2(450, 440);
    PanelBg->SetMaterialProperty("Color", SVector4(0.12f, 0.12f, 0.18f));
    PanelBg->SetMaterialProperty("CornerRadius", SVector4(16));
    Panel->GetSlotForChild(PanelBg)->Alignment = SVector4(0, 1, 0, 1);

    WVerticalBox* VBox = Panel->AddChild<WVerticalBox>();
    Panel->GetSlotForChild(VBox)->Alignment = SVector4(0, 1, 0, 1);
    Panel->GetSlotForChild(VBox)->Padding = SVector4(30);

    // Title
    WText* Title = VBox->AddChild<WText>();
    Title->SetText("Ocean Cleanup");
    Title->FontSize = 42.0f;
    Title->SetMaterialProperty("Color", SVector4(0.2f, 0.8f, 1.0f));
    VBox->GetSlotForChild(Title)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(Title)->Padding = SVector4(0, 0, 0, 15);

    // Username label
    WText* Label = VBox->AddChild<WText>();
    Label->SetText("Enter Username:");
    Label->FontSize = 22.0f;
    Label->SetMaterialProperty("Color", SVector4(0.7f, 0.7f, 0.7f));
    VBox->GetSlotForChild(Label)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(Label)->Padding = SVector4(0, 0, 0, 5);

    // Input box with background
    WOverlay* InputContainer = VBox->AddChild<WOverlay>();
    VBox->GetSlotForChild(InputContainer)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(InputContainer)->Padding = SVector4(0, 0, 0, 15);

    WImage* InputBorder = InputContainer->AddChild<WImage>();
    InputBorder->Size = SVector2(300, 45);
    InputBorder->SetMaterialProperty("Color", SVector4(0.2f, 0.2f, 0.3f));
    InputBorder->SetMaterialProperty("CornerRadius", SVector4(8));
    InputContainer->GetSlotForChild(InputBorder)->Alignment = SVector4(0, 1, 0, 1);

    WInputBox* InputBox = InputContainer->AddChild<WInputBox>();
    InputBox->FontSize = 24.0f;
    InputBox->MaxLength = 20;
    InputBox->SetMaterialProperty("Color", SVector4(1.0f));
    InputContainer->GetSlotForChild(InputBox)->Alignment = SVector4(0, 1, 0, 1);
    InputContainer->GetSlotForChild(InputBox)->Padding = SVector4(10, 8, 10, 8);

    WInputBox* InputBoxRef = InputBox;

    // Play button
    WButton* PlayBtn = VBox->AddChild<WButton>();
    PlayBtn->Size = SVector2(260, 50);
    PlayBtn->SetText("Play Game");
    PlayBtn->SetFontSize(24.0f);
    PlayBtn->SetTextMaterialProperty("Color", SVector4(1));
    PlayBtn->GetImageWidget()->SetMaterialProperty("Color", SVector4(0.15f, 0.55f, 0.25f));
    PlayBtn->GetImageWidget()->SetMaterialProperty("CornerRadius", SVector4(8));
    VBox->GetSlotForChild(PlayBtn)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(PlayBtn)->Padding = SVector4(0, 5, 0, 5);

    auto StartGame = [InputBoxRef]() {
        std::string Name = InputBoxRef->GetText();
        if (Name.empty()) Name = "Player";
        CGameInstance* GI = GEngine->GetGameInstance<CGameInstance>();
        GI->Username = Name;
        GI->LoadLeaderboard();
        GEngine->GetRenderer()->ClearUIWidgets();
        GEngine->GetRenderer()->AddUIWidget<WLoadingScreen>();
        GEngine->OpenWorld(CGameMap::StaticClass());
    };

    PlayBtn->OnClickedDelegate.Bind(StartGame);
    InputBox->OnTextConfirmed.Bind([StartGame](const std::string&) { StartGame(); });

    // Leaderboard button
    WButton* LeaderboardBtn = VBox->AddChild<WButton>();
    LeaderboardBtn->Size = SVector2(260, 50);
    LeaderboardBtn->SetText("Leaderboard");
    LeaderboardBtn->SetFontSize(24.0f);
    LeaderboardBtn->SetTextMaterialProperty("Color", SVector4(1));
    LeaderboardBtn->GetImageWidget()->SetMaterialProperty("Color", SVector4(0.2f, 0.35f, 0.6f));
    LeaderboardBtn->GetImageWidget()->SetMaterialProperty("CornerRadius", SVector4(8));
    VBox->GetSlotForChild(LeaderboardBtn)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(LeaderboardBtn)->Padding = SVector4(0, 5, 0, 5);
    LeaderboardBtn->OnClickedDelegate.Bind([]() {
        GEngine->GetRenderer()->ClearUIWidgets();
        GEngine->GetRenderer()->AddUIWidget<WLeaderboardUI>();
    });

    // Quit button
    WButton* QuitBtn = VBox->AddChild<WButton>();
    QuitBtn->Size = SVector2(260, 50);
    QuitBtn->SetText("Quit Game");
    QuitBtn->SetFontSize(24.0f);
    QuitBtn->SetTextMaterialProperty("Color", SVector4(1));
    QuitBtn->GetImageWidget()->SetMaterialProperty("Color", SVector4(0.55f, 0.15f, 0.15f));
    QuitBtn->GetImageWidget()->SetMaterialProperty("CornerRadius", SVector4(8));
    VBox->GetSlotForChild(QuitBtn)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(QuitBtn)->Padding = SVector4(0, 5, 0, 5);
    QuitBtn->OnClickedDelegate.Bind([]() { GEngine->Stop(); });
}
