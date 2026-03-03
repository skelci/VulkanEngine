#pragma once

#include "ContainerBase.hpp"


struct SHorizontalBoxSlot {
    // Vertical alignment within the row.
    // X = Top alignment, Y = Bottom alignment
    // (0, 0) = Top, (0, 1) = Stretch, (1, 1) = Bottom, (0.5, 0.5) = Center
    SVector2 Alignment = SVector2(0, 1);

    // Left, Top, Right, Bottom
    SVector4 Padding;
};


class WHorizontalBox : public WContainerBase {
    CLASS_BODY(WHorizontalBox, WContainerBase)

public:
    virtual SVector2 GetDesiredSize() const override;
    virtual SWidgetTransform GetChildTransform(const WWidget* Child) const override;
    SHorizontalBoxSlot* GetSlotForChild(const WWidget* Child);

protected:
    virtual void OnChildAdded(WWidget* Widget) override;
    std::vector<SHorizontalBoxSlot> Slots;
};
