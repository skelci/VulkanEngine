#pragma once

#include "ContainerBase.hpp"


struct SOverlaySlot {
    // Left, Top, Right, Bottom
    SVector4 Padding;

    // 0.0 to 1.0, relative to parent size.
    // Left must be less than Right, Top must be less than Bottom.
    // If Left + Right < 1.0, the widget will be stretched to fill the remaining space.
    // 0.0 is relative size from the left/top, even in Right/Bottom
    // X = Left alignment, Y = Right alignment)
    // Z = Top alignment, W = Bottom alignment
    SVector4 Alignment;

    // Offset from final calculated position
    SVector2 Offset;
};


class WOverlay : public WContainerBase {
    CLASS_BODY(WOverlay, WContainerBase)

public:
    virtual SVector2 GetDesiredSize() const override;
    virtual SWidgetTransform GetChildTransform(const WWidget* Child) const override;
    SOverlaySlot* GetSlotForChild(const WWidget* Child);

protected:
    virtual void OnChildAdded(WWidget* Widget) override;
    std::vector<SOverlaySlot> Slots;
};
