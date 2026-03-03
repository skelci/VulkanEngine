#include "VerticalBox.hpp"

#include "EngineStatics.hpp"


SVector2 WVerticalBox::GetDesiredSize() const {
    SVector2 TotalSize(0, 0);
    for (int32 i = 0; i < Children.size(); i++) {
        const WWidget* Child = Children[i].get();
        const SVerticalBoxSlot& Slot = Slots[i];
        SVector2 ChildSize = Child->GetDesiredSize();
        ChildSize.X += Slot.Padding.X + Slot.Padding.Z;
        ChildSize.Y += Slot.Padding.Y + Slot.Padding.W;
        TotalSize.X = std::max(TotalSize.X, ChildSize.X);
        TotalSize.Y += ChildSize.Y;
    }
    return TotalSize;
}

SWidgetTransform WVerticalBox::GetChildTransform(const WWidget* Child) const {
    const int32 Index = GetChildIndex(Child);
    if (Index < 0) {
        Log("WVerticalBox", ELogLevel::Warning,
            "GetChildTransform called with a widget that is not a child of this box.");
        return SWidgetTransform();
    }

    const SWidgetTransform ParentTransform = GetTransform();

    float YOffset = 0.0f;
    for (int32 i = 0; i < Index; i++) {
        const SVerticalBoxSlot& PrevSlot = Slots[i];
        const float PrevHeight = Children[i]->GetDesiredSize().Y + PrevSlot.Padding.Y + PrevSlot.Padding.W;
        YOffset += PrevHeight;
    }

    const SVerticalBoxSlot& Slot = Slots[Index];
    const SVector2 ChildDesiredSize = Child->GetDesiredSize();

    const float AvailableWidth = ParentTransform.Size.X - Slot.Padding.X - Slot.Padding.Z;
    const float WidthDiff = AvailableWidth - ChildDesiredSize.X;
    const float Scale = Slot.Alignment.Y - Slot.Alignment.X;

    const float ChildX = ParentTransform.Position.X + Slot.Padding.X + WidthDiff * Slot.Alignment.X;
    const float ChildY = ParentTransform.Position.Y + YOffset + Slot.Padding.Y;
    const float ChildWidth = ChildDesiredSize.X + WidthDiff * Scale;
    const float ChildHeight = ChildDesiredSize.Y;

    return SWidgetTransform{SVector2(ChildX, ChildY), SVector2(ChildWidth, ChildHeight)};
}

SVerticalBoxSlot* WVerticalBox::GetSlotForChild(const WWidget* Child) {
    const int32 Index = GetChildIndex(Child);
    if (Index < 0) {
        Log("WVerticalBox", ELogLevel::Warning,
            "GetSlotForChild called with a widget that is not a child of this box.");
        return nullptr;
    }
    return &Slots[Index];
}

void WVerticalBox::OnChildAdded(WWidget* Widget) { Slots.push_back(SVerticalBoxSlot()); }
