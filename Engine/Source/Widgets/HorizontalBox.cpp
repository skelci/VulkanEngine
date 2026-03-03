#include "HorizontalBox.hpp"

#include "EngineStatics.hpp"


SVector2 WHorizontalBox::GetDesiredSize() const {
    SVector2 TotalSize(0, 0);
    for (int32 i = 0; i < Children.size(); i++) {
        const WWidget* Child = Children[i].get();
        const SHorizontalBoxSlot& Slot = Slots[i];
        SVector2 ChildSize = Child->GetDesiredSize();
        ChildSize.X += Slot.Padding.X + Slot.Padding.Z;
        ChildSize.Y += Slot.Padding.Y + Slot.Padding.W;
        TotalSize.X += ChildSize.X;
        TotalSize.Y = std::max(TotalSize.Y, ChildSize.Y);
    }
    return TotalSize;
}

SWidgetTransform WHorizontalBox::GetChildTransform(const WWidget* Child) const {
    const int32 Index = GetChildIndex(Child);
    if (Index < 0) {
        Log("WHorizontalBox", ELogLevel::Warning,
            "GetChildTransform called with a widget that is not a child of this box.");
        return SWidgetTransform();
    }

    const SWidgetTransform ParentTransform = GetTransform();

    float XOffset = 0.0f;
    for (int32 i = 0; i < Index; i++) {
        const SHorizontalBoxSlot& PrevSlot = Slots[i];
        const float PrevWidth = Children[i]->GetDesiredSize().X + PrevSlot.Padding.X + PrevSlot.Padding.Z;
        XOffset += PrevWidth;
    }

    const SHorizontalBoxSlot& Slot = Slots[Index];
    const SVector2 ChildDesiredSize = Child->GetDesiredSize();

    const float AvailableHeight = ParentTransform.Size.Y - Slot.Padding.Y - Slot.Padding.W;
    const float HeightDiff = AvailableHeight - ChildDesiredSize.Y;
    const float Scale = Slot.Alignment.Y - Slot.Alignment.X;

    const float ChildX = ParentTransform.Position.X + XOffset + Slot.Padding.X;
    const float ChildY = ParentTransform.Position.Y + Slot.Padding.Y + HeightDiff * Slot.Alignment.X;
    const float ChildWidth = ChildDesiredSize.X;
    const float ChildHeight = ChildDesiredSize.Y + HeightDiff * Scale;

    return SWidgetTransform{SVector2(ChildX, ChildY), SVector2(ChildWidth, ChildHeight)};
}

SHorizontalBoxSlot* WHorizontalBox::GetSlotForChild(const WWidget* Child) {
    const int32 Index = GetChildIndex(Child);
    if (Index < 0) {
        Log("WHorizontalBox", ELogLevel::Warning,
            "GetSlotForChild called with a widget that is not a child of this box.");
        return nullptr;
    }
    return &Slots[Index];
}

void WHorizontalBox::OnChildAdded(WWidget* Widget) { Slots.push_back(SHorizontalBoxSlot()); }
