#include "MainMenu.hpp"

#include "Maps/GameMap.hpp"

#include "Assets/Material.hpp"
#include "Widgets/Image.hpp"
#include "Widgets/InputBox.hpp"
#include "Widgets/Text.hpp"


WMainMenu::WMainMenu() {
    WImage* bg = AddChild<WImage>();
    bg->SetMaterialProperty("Color", SVector4(0.2f, 0.05f, 0.25f));
    GetSlotForChild(bg)->Alignment = SVector4(0, 1, 0, 1);

    WOverlay* usernameOverlay = AddChild<WOverlay>();
    GetSlotForChild(usernameOverlay)->Alignment = SVector4(0.5f);

    WImage* border = usernameOverlay->AddChild<WImage>();
    border->Size = SVector2(400, 60);
    border->SetMaterialProperty("Color", SVector4(0.8f, 0.8f, 0.8f));
    border->SetMaterialProperty("BorderWidth", 5.0f);
    border->SetMaterialProperty("BorderColor", SVector4(0.1f, 0.8f, 0.1f));
    border->SetMaterialProperty("CornerRadius", SVector4(10));
    SOverlaySlot* borderSlot = usernameOverlay->GetSlotForChild(border);
    borderSlot->Alignment = SVector4(0.0f, 1.0f, 0.5f, 0.5f);
    borderSlot->Offset = SVector2(200, 0);

    WText* username = usernameOverlay->AddChild<WText>();
    username->FontSize = 36.0f;
    username->SetText("Enter Username: ");
    username->SetMaterialProperty("Color", SVector4(0.9f, 0.2f, 0.2f));
    SOverlaySlot* usernameSlot = usernameOverlay->GetSlotForChild(username);
    usernameSlot->Alignment = SVector4(1.0f, 0.5f);
    usernameSlot->Offset = SVector2(-200, 0);

    WInputBox* inputBox = usernameOverlay->AddChild<WInputBox>();
    inputBox->FontSize = 36.0f;
    inputBox->MaxLength = 25;
    inputBox->SetMaterialProperty("Color", SVector4(0.0f, 0.0f, 0.0f));
    SOverlaySlot* inputSlot = usernameOverlay->GetSlotForChild(inputBox);
    inputSlot->Alignment = SVector4(0.0f, 1.0f, 0.5f, 0.5f);
    inputSlot->Offset = SVector2(200, 0);
    inputSlot->Padding = SVector4(10);
    inputBox->OnTextConfirmed.Bind([this](const std::string& Text) {
        Log("MainMenu", ELogLevel::Info, "Username entered: " + Text);
        GEngine->OpenWorld(CGameMap::StaticClass());
        GEngine->GetRenderer()->RemoveUIWidget(this);
    });
}
