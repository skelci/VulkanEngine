#pragma once

#include "Delegate.hpp"
#include "Object.hpp"
#include "Vector.hpp"

class WContainerBase;


struct SWidgetTransform {
    SVector2 Position;
    SVector2 Size;
};


class WWidget : public CObject {
    CLASS_BODY(WWidget, CObject)

public:
    WWidget();
    ~WWidget();

    virtual SVector2 GetDesiredSize() const;
    SWidgetTransform GetTransform() const;

    WContainerBase* GetParent() const { return Parent; }

    template <typename T>
    T* GetParent() const {
        return Cast<T>(Parent);
    }

private:
    friend WContainerBase;
    WContainerBase* Parent = nullptr;

protected:
    virtual void OnCursorMoved(SVector2 Delta);
    virtual void OnMouseDown();
    virtual void OnMouseUp();

    virtual void OnHovered() {}
    virtual void OnUnhovered() {}
    virtual void OnPressed() {}
    virtual void OnReleased() {}
    virtual void OnClicked();
    virtual void OnFocused();
    virtual void OnUnfocused();

    struct SInputMappingContext* InputContext;
    bool IsMouseOver = false;
    bool IsFocused = false;
    bool IsPressed = false;

public:
    bool GetIsHovered() const { return IsMouseOver && !IsPressed; }
    bool GetIsPressed() const { return IsPressed; }
    bool GetIsFocused() const { return IsFocused; }

    TDelegate<> OnClickedDelegate;
};
