#include "Overlay.hpp"

#include "EngineStatics.hpp"


SVector2 WOverlay::GetDesiredSize() const {
    SVector2 MaxSize(0, 0);
    for (int32 i = 0; i < Children.size(); i++) {
        const WWidget* Child = Children[i].get();
        const SOverlaySlot& Slot = Slots[i];
        SVector2 ChildSize = Child->GetDesiredSize() + Slot.Padding.XY() + Slot.Padding.ZW();
        MaxSize.X = std::max(MaxSize.X, ChildSize.X);
        MaxSize.Y = std::max(MaxSize.Y, ChildSize.Y);
    }
    return MaxSize;
}

SWidgetTransform WOverlay::GetChildTransform(const WWidget* Child) const {
    const int32 Index = GetChildIndex(Child);
    if (Index < 0) {
        Log("WOverlay", ELogLevel::Warning,
            "GetChildTransform called with a widget that is not a child of this overlay.");
        return SWidgetTransform();
    }

    const SOverlaySlot& Slot = Slots[Index];
    const SVector2 ChildDesiredSize = Child->GetDesiredSize();
    const SWidgetTransform ParentTransform = GetTransform();
    const SVector2 AvailableSize = ParentTransform.Size - Slot.Padding.XY() - Slot.Padding.ZW() - ChildDesiredSize;
    const SVector2 Scale = SVector2(Slot.Alignment.Y - Slot.Alignment.X, Slot.Alignment.W - Slot.Alignment.Z);
    return SWidgetTransform{
        ParentTransform.Position + Slot.Padding.XY() + AvailableSize * Slot.Alignment.XZ() + Slot.Offset,
        ChildDesiredSize + AvailableSize * Scale
    };
}

SOverlaySlot* WOverlay::GetSlotForChild(const WWidget* Child) {
    const int32 Index = GetChildIndex(Child);
    if (Index < 0) {
        Log("WOverlay", ELogLevel::Warning,
            "GetSlotForChild called with a widget that is not a child of this overlay.");
        return nullptr;
    }
    return &Slots[Index];
}

void WOverlay::OnChildAdded(WWidget* Widget) { Slots.push_back(SOverlaySlot()); }
