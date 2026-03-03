#include "Button.hpp"

#include "Image.hpp"
#include "Text.hpp"

#include "EngineStatics.hpp"


WButton::WButton() {
    ImageWidget = AddChild<WImage>();
    SOverlaySlot* ImageSlot = GetSlotForChild(ImageWidget);
    ImageSlot->Alignment = SVector4(0, 1, 0, 1);

    TextWidget = AddChild<WText>();
    SOverlaySlot* TextSlot = GetSlotForChild(TextWidget);
    TextSlot->Alignment = SVector4(0.5f);
}

void WButton::SetNormalImage(std::shared_ptr<CMaterial> Material) {
    NormalMaterial = Material;
    UpdateImage();
}

void WButton::SetHoveredImage(std::shared_ptr<CMaterial> Material) {
    HoveredMaterial = Material;
    UpdateImage();
}

void WButton::SetPressedImage(std::shared_ptr<CMaterial> Material) {
    PressedMaterial = Material;
    UpdateImage();
}

void WButton::SetText(const std::string& InText) { TextWidget->SetText(InText); }

void WButton::SetFont(std::shared_ptr<CFont> InFont) { TextWidget->SetFont(InFont); }

void WButton::SetFontSize(float InSize) { TextWidget->FontSize = InSize; }

void WButton::SetTextMaterial(std::shared_ptr<CMaterial> Material) { TextWidget->SetMaterial(Material); }

void WButton::OnHovered() {
    WOverlay::OnHovered();
    UpdateImage();
}

void WButton::OnUnhovered() {
    WOverlay::OnUnhovered();
    UpdateImage();
}

void WButton::OnPressed() {
    WOverlay::OnPressed();
    UpdateImage();
}

void WButton::OnReleased() {
    WOverlay::OnReleased();
    UpdateImage();
}

void WButton::UpdateImage() {
    if (IsPressed && PressedMaterial) {
        ImageWidget->SetMaterial(PressedMaterial);
    } else if (IsMouseOver && HoveredMaterial) {
        ImageWidget->SetMaterial(HoveredMaterial);
    } else if (NormalMaterial) {
        ImageWidget->SetMaterial(NormalMaterial);
    }
}
