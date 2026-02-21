#include "InputBox.hpp"


void WInputBox::OnCursorMoved(SVector2 Delta) {
    Super::OnCursorMoved(Delta);
    const SWidgetTransform Transform = GetTransform();
    const SVector2 CursorPos = GInputManager->GetCursorPosition();
}

void WInputBox::OnFocused() {
    Super::OnFocused();
    GInputManager->OnCharInput.Bind(this, &WInputBox::OnCharInput);
}

void WInputBox::OnUnfocused() {
    Super::OnUnfocused();
    GInputManager->OnCharInput.Unbind(this, &WInputBox::OnCharInput);
}

void WInputBox::OnCharInput(char Character) {
    if (Character >= 32 && Character <= 126) {
        if (Text.size() < MaxLength) {
            Text += Character;
            RebuildMesh();
        }
    } else if (Character == '\b') {
        if (!Text.empty()) {
            Text.pop_back();
            RebuildMesh();
        }
    } else if (Character == '\n') {
        OnUnfocused();
        OnTextConfirmed.Broadcast(Text);
    }
}
