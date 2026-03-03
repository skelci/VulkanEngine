#pragma once

#include "ContainerBase.hpp"


struct SVerticalBoxSlot {
    // Horizontal alignment within the column.
    // X = Left alignment, Y = Right alignment
    // (0, 0) = Left, (0, 1) = Stretch, (1, 1) = Right, (0.5, 0.5) = Center
    SVector2 Alignment = SVector2(0, 1);

    // Left, Top, Right, Bottom
    SVector4 Padding;
};


class WVerticalBox : public WContainerBase {
    CLASS_BODY(WVerticalBox, WContainerBase)

public:
    virtual SVector2 GetDesiredSize() const override;
    virtual SWidgetTransform GetChildTransform(const WWidget* Child) const override;
    SVerticalBoxSlot* GetSlotForChild(const WWidget* Child);

protected:
    virtual void OnChildAdded(WWidget* Widget) override;
    std::vector<SVerticalBoxSlot> Slots;
};
