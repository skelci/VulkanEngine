#include "Widget.hpp"

#include "ContainerBase.hpp"
#include "EngineStatics.hpp"


WWidget::WWidget() {
    InputContext = new SInputMappingContext();
    GInputManager->AddMappingContext(InputContext);
    InputContext->AddMapping(
        EKeys::Mouse2D,
        MAKE_INPUT_ACTION(EInputValueType::Axis2D, EInputEvent::Triggered, this, &WWidget::OnCursorMoved)
    );
    InputContext->AddMapping(
        EKeys::MouseLeft, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Pressed, this, &WWidget::OnMouseDown)
    );
    InputContext->AddMapping(
        EKeys::MouseLeft, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Released, this, &WWidget::OnMouseUp)
    );
}

WWidget::~WWidget() {
    if (InputContext) {
        GInputManager->RemoveMappingContext(InputContext);
        InputContext = nullptr;
    }
}

SVector2 WWidget::GetDesiredSize() const { return SVector2(); }

SWidgetTransform WWidget::GetTransform() const {
    if (!Parent) {
        return SWidgetTransform{SVector2(), GEngine->GetRenderer()->GetScreenSize()};
    }
    return Parent->GetChildTransform(this);
}

void WWidget::OnCursorMoved(SVector2 Delta) {
    const SWidgetTransform Transform = GetTransform();
    const SVector2 CursorPos = GInputManager->GetCursorPosition();
    const bool wasMouseOver = IsMouseOver;
    IsMouseOver = CursorPos.X >= Transform.Position.X && CursorPos.X <= Transform.Position.X + Transform.Size.X &&
                  CursorPos.Y >= Transform.Position.Y && CursorPos.Y <= Transform.Position.Y + Transform.Size.Y;
    if (wasMouseOver && !IsMouseOver) {
        OnUnhovered();
        if (IsPressed) {
            IsPressed = false;
            OnReleased();
        }
    } else if (!wasMouseOver && IsMouseOver) {
        OnHovered();
    }
}

void WWidget::OnMouseDown() {
    if (IsMouseOver) {
        IsPressed = true;
        if (!IsFocused) {
            OnFocused();
        }
        OnPressed();
    } else if (IsFocused) {
        OnUnfocused();
    }
}

void WWidget::OnMouseUp() {
    if (IsPressed) {
        OnReleased();
        OnClicked();
    }
    IsPressed = false;
}

void WWidget::OnClicked() { OnClickedDelegate.Broadcast(); }

void WWidget::OnFocused() { IsFocused = true; }

void WWidget::OnUnfocused() { IsFocused = false; }
