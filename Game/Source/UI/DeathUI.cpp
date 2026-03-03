#include "DeathUI.hpp"

#include "Maps/GameMap.hpp"
#include "Maps/MenuMap.hpp"
#include "UI/LoadingScreen.hpp"

#include "Widgets/Button.hpp"
#include "Widgets/Image.hpp"
#include "Widgets/Text.hpp"
#include "Widgets/VerticalBox.hpp"


WDeathUI::WDeathUI() {
    // Dark red overlay
    WImage* Bg = AddChild<WImage>();
    Bg->SetMaterialProperty("Color", SVector4(0.1f, 0.0f, 0.0f, 0.8f));
    GetSlotForChild(Bg)->Alignment = SVector4(0, 1, 0, 1);

    // Center panel
    WOverlay* Panel = AddChild<WOverlay>();
    GetSlotForChild(Panel)->Alignment = SVector4(0.5f);

    WImage* PanelBg = Panel->AddChild<WImage>();
    PanelBg->Size = SVector2(400, 300);
    PanelBg->SetMaterialProperty("Color", SVector4(0.18f, 0.08f, 0.08f));
    PanelBg->SetMaterialProperty("CornerRadius", SVector4(16));
    Panel->GetSlotForChild(PanelBg)->Alignment = SVector4(0, 1, 0, 1);

    WVerticalBox* VBox = Panel->AddChild<WVerticalBox>();
    Panel->GetSlotForChild(VBox)->Alignment = SVector4(0, 1, 0, 1);
    Panel->GetSlotForChild(VBox)->Padding = SVector4(30);

    // Title
    WText* Title = VBox->AddChild<WText>();
    Title->SetText("You Died!");
    Title->FontSize = 40.0f;
    Title->SetMaterialProperty("Color", SVector4(1.0f, 0.3f, 0.3f));
    VBox->GetSlotForChild(Title)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(Title)->Padding = SVector4(0, 0, 0, 10);

    // Subtitle
    WText* Subtitle = VBox->AddChild<WText>();
    Subtitle->SetText("Enemies overwhelmed you!");
    Subtitle->FontSize = 20.0f;
    Subtitle->SetMaterialProperty("Color", SVector4(0.7f, 0.5f, 0.5f));
    VBox->GetSlotForChild(Subtitle)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(Subtitle)->Padding = SVector4(0, 0, 0, 25);

    // Restart button
    WButton* RestartBtn = VBox->AddChild<WButton>();
    RestartBtn->Size = SVector2(250, 50);
    RestartBtn->SetText("Restart Level");
    RestartBtn->SetFontSize(24.0f);
    RestartBtn->SetTextMaterialProperty("Color", SVector4(1));
    RestartBtn->GetImageWidget()->SetMaterialProperty("Color", SVector4(0.6f, 0.25f, 0.15f));
    RestartBtn->GetImageWidget()->SetMaterialProperty("CornerRadius", SVector4(8));
    VBox->GetSlotForChild(RestartBtn)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(RestartBtn)->Padding = SVector4(0, 5, 0, 5);
    RestartBtn->OnClickedDelegate.Bind([]() {
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
    MenuBtn->OnClickedDelegate.Bind([]() {
        GEngine->GetRenderer()->ClearUIWidgets();
        GEngine->GetRenderer()->AddUIWidget<WLoadingScreen>();
        GEngine->OpenWorld(CMenuMap::StaticClass());
    });
}
