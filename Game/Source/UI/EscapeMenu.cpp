#include "EscapeMenu.hpp"

#include "Maps/GameMap.hpp"
#include "Maps/MenuMap.hpp"
#include "UI/LoadingScreen.hpp"

#include "Widgets/Button.hpp"
#include "Widgets/Image.hpp"
#include "Widgets/Text.hpp"
#include "Widgets/VerticalBox.hpp"


WEscapeMenu::WEscapeMenu() {
    // Semi-transparent dark overlay
    WImage* Bg = AddChild<WImage>();
    Bg->SetMaterialProperty("Color", SVector4(0.0f, 0.0f, 0.0f, 0.7f));
    GetSlotForChild(Bg)->Alignment = SVector4(0, 1, 0, 1);

    // Center panel
    WOverlay* Panel = AddChild<WOverlay>();
    GetSlotForChild(Panel)->Alignment = SVector4(0.5f);

    WImage* PanelBg = Panel->AddChild<WImage>();
    PanelBg->Size = SVector2(350, 250);
    PanelBg->SetMaterialProperty("Color", SVector4(0.15f, 0.15f, 0.2f));
    PanelBg->SetMaterialProperty("CornerRadius", SVector4(12));
    Panel->GetSlotForChild(PanelBg)->Alignment = SVector4(0, 1, 0, 1);

    WVerticalBox* VBox = Panel->AddChild<WVerticalBox>();
    Panel->GetSlotForChild(VBox)->Alignment = SVector4(0, 1, 0, 1);
    Panel->GetSlotForChild(VBox)->Padding = SVector4(30);

    // Title
    WText* Title = VBox->AddChild<WText>();
    Title->SetText("Paused");
    Title->FontSize = 36.0f;
    Title->SetMaterialProperty("Color", SVector4(1.0f));
    VBox->GetSlotForChild(Title)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(Title)->Padding = SVector4(0, 0, 0, 20);

    // Resume button
    WButton* ResumeBtn = VBox->AddChild<WButton>();
    ResumeBtn->Size = SVector2(250, 50);
    ResumeBtn->SetText("Resume");
    ResumeBtn->SetFontSize(24.0f);
    ResumeBtn->SetTextMaterialProperty("Color", SVector4(1));
    ResumeBtn->GetImageWidget()->SetMaterialProperty("Color", SVector4(0.2f, 0.5f, 0.2f));
    ResumeBtn->GetImageWidget()->SetMaterialProperty("CornerRadius", SVector4(8));
    VBox->GetSlotForChild(ResumeBtn)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(ResumeBtn)->Padding = SVector4(0, 5, 0, 5);
    ResumeBtn->OnClickedDelegate.Bind([]() {
        CGameMap* GameMap = Cast<CGameMap>(GEngine->GetWorld());
        if (GameMap) GameMap->ToggleEscapeMenu();
    });

    // Main Menu button
    WButton* MenuBtn = VBox->AddChild<WButton>();
    MenuBtn->Size = SVector2(250, 50);
    MenuBtn->SetText("Main Menu");
    MenuBtn->SetFontSize(24.0f);
    MenuBtn->SetTextMaterialProperty("Color", SVector4(1));
    MenuBtn->GetImageWidget()->SetMaterialProperty("Color", SVector4(0.5f, 0.2f, 0.2f));
    MenuBtn->GetImageWidget()->SetMaterialProperty("CornerRadius", SVector4(8));
    VBox->GetSlotForChild(MenuBtn)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(MenuBtn)->Padding = SVector4(0, 5, 0, 5);
    MenuBtn->OnClickedDelegate.Bind([]() {
        GEngine->GetRenderer()->ClearUIWidgets();
        GEngine->GetRenderer()->AddUIWidget<WLoadingScreen>();
        GEngine->OpenWorld(CMenuMap::StaticClass());
    });
}
