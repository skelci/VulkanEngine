#pragma once

#include "Overlay.hpp"
#include "Text.hpp"

#include <memory>

class WImage;
class CMaterial;
class CFont;


class WButton : public WOverlay {
    CLASS_BODY(WButton, WOverlay)

public:
    WButton();

    SVector2 Size = SVector2(150, 40);
    virtual SVector2 GetDesiredSize() const override { return Size; }

    void SetNormalImage(std::shared_ptr<CMaterial> Material);
    void SetHoveredImage(std::shared_ptr<CMaterial> Material);
    void SetPressedImage(std::shared_ptr<CMaterial> Material);

    void SetText(const std::string& InText);
    void SetFont(std::shared_ptr<CFont> InFont);
    void SetFontSize(float InSize);
    void SetTextMaterial(std::shared_ptr<CMaterial> Material);

    template <typename T>
    void SetTextMaterialProperty(const std::string& Name, const T& Value) {
        TextWidget->SetMaterialProperty(Name, Value);
    }

    WImage* GetImageWidget() const { return ImageWidget; }
    WText* GetTextWidget() const { return TextWidget; }

protected:
    virtual void OnHovered() override;
    virtual void OnUnhovered() override;
    virtual void OnPressed() override;
    virtual void OnReleased() override;

    void UpdateImage();

    WImage* ImageWidget = nullptr;
    WText* TextWidget = nullptr;

    std::shared_ptr<CMaterial> NormalMaterial;
    std::shared_ptr<CMaterial> HoveredMaterial;
    std::shared_ptr<CMaterial> PressedMaterial;
};
