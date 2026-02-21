#pragma once

#include "Text.hpp"


class WInputBox : public WText {
    CLASS_BODY(WInputBox, WText)

public:
    int32 MaxLength = 20;
    TDelegate<const std::string&> OnTextConfirmed;

protected:
    virtual void OnCursorMoved(SVector2 Delta) override;
    virtual void OnFocused() override;
    virtual void OnUnfocused() override;

    virtual void OnCharInput(char Character);
};
