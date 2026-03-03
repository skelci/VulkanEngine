#include "LoadingScreen.hpp"

#include "Widgets/Image.hpp"
#include "Widgets/Text.hpp"


WLoadingScreen::WLoadingScreen() {
    WImage* Bg = AddChild<WImage>();
    Bg->SetMaterialProperty("Color", SVector4(0.05f, 0.05f, 0.1f));
    GetSlotForChild(Bg)->Alignment = SVector4(0, 1, 0, 1);

    WText* LoadingText = AddChild<WText>();
    LoadingText->SetText("Loading...");
    LoadingText->FontSize = 48.0f;
    LoadingText->SetMaterialProperty("Color", SVector4(0.8f, 0.8f, 0.8f));
    GetSlotForChild(LoadingText)->Alignment = SVector4(0.5f);
}
