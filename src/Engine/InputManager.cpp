#include "InputManager.hpp"

#include "CoreMinimal.hpp"
#include "GLFW/glfw3.h"

#include <algorithm>
#include <cassert>


static const std::unordered_map<EKeys, uint32_t> KeyToGLFWCode = {
    {EKeys::A, GLFW_KEY_A},
    {EKeys::B, GLFW_KEY_B},
    {EKeys::C, GLFW_KEY_C},
    {EKeys::D, GLFW_KEY_D},
    {EKeys::E, GLFW_KEY_E},
    {EKeys::F, GLFW_KEY_F},
    {EKeys::G, GLFW_KEY_G},
    {EKeys::H, GLFW_KEY_H},
    {EKeys::I, GLFW_KEY_I},
    {EKeys::J, GLFW_KEY_J},
    {EKeys::K, GLFW_KEY_K},
    {EKeys::L, GLFW_KEY_L},
    {EKeys::M, GLFW_KEY_M},
    {EKeys::N, GLFW_KEY_N},
    {EKeys::O, GLFW_KEY_O},
    {EKeys::P, GLFW_KEY_P},
    {EKeys::Q, GLFW_KEY_Q},
    {EKeys::R, GLFW_KEY_R},
    {EKeys::S, GLFW_KEY_S},
    {EKeys::T, GLFW_KEY_T},
    {EKeys::U, GLFW_KEY_U},
    {EKeys::V, GLFW_KEY_V},
    {EKeys::W, GLFW_KEY_W},
    {EKeys::X, GLFW_KEY_X},
    {EKeys::Y, GLFW_KEY_Y},
    {EKeys::Z, GLFW_KEY_Z},
    {EKeys::Zero, GLFW_KEY_0},
    {EKeys::One, GLFW_KEY_1},
    {EKeys::Two, GLFW_KEY_2},
    {EKeys::Three, GLFW_KEY_3},
    {EKeys::Four, GLFW_KEY_4},
    {EKeys::Five, GLFW_KEY_5},
    {EKeys::Six, GLFW_KEY_6},
    {EKeys::Seven, GLFW_KEY_7},
    {EKeys::Eight, GLFW_KEY_8},
    {EKeys::Nine, GLFW_KEY_9},
    {EKeys::Escape, GLFW_KEY_ESCAPE},
    {EKeys::LeftControl, GLFW_KEY_LEFT_CONTROL},
    {EKeys::LeftShift, GLFW_KEY_LEFT_SHIFT},
    {EKeys::LeftAlt, GLFW_KEY_LEFT_ALT},
    {EKeys::RightControl, GLFW_KEY_RIGHT_CONTROL},
    {EKeys::RightShift, GLFW_KEY_RIGHT_SHIFT},
    {EKeys::RightAlt, GLFW_KEY_RIGHT_ALT},
    {EKeys::Space, GLFW_KEY_SPACE},
    {EKeys::Enter, GLFW_KEY_ENTER},
    {EKeys::Backspace, GLFW_KEY_BACKSPACE},
    {EKeys::Tab, GLFW_KEY_TAB},
    {EKeys::MouseLeft, GLFW_MOUSE_BUTTON_LEFT},
    {EKeys::MouseRight, GLFW_MOUSE_BUTTON_RIGHT},
    {EKeys::MouseMiddle, GLFW_MOUSE_BUTTON_MIDDLE},
};

CInputManager::CInputManager() {
    glfwSetScrollCallback(GEngine->GetWindow(), [](GLFWwindow*, double xoff, double yoff) {
        CInputManager::OnScroll(static_cast<float>(yoff));
    });
    glfwSetCursorPosCallback(GEngine->GetWindow(), [](GLFWwindow*, double x, double y) {
        CInputManager::OnCursor(SVector2(x, y));
    });

    glfwSetInputMode(GEngine->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void CInputManager::Tick(float DeltaTime) {
    std::sort(
        MappingContexts.begin(), MappingContexts.end(),
        [](const std::shared_ptr<SInputMappingContext>& a, const std::shared_ptr<SInputMappingContext>& b) {
            return b->Priority >
                   a->Priority; // Lower priority because it could be overridden by higher priority contexts
        }
    );

    for (const auto& context : MappingContexts) {
        for (const auto& [key, action] : context->GetMappings()) {
            switch (action.ValueType) {
            case EInputValueType::Digital: ProcessDigitalInput(key, action); break;
            case EInputValueType::Axis1D: ProcessAxis1DInput(key, action); break;
            case EInputValueType::Axis2D: ProcessAxis2DInput(key, action); break;
            }
        }
    }

    PreviousFrameDigitalKeys = ThisFrameDigitalKeys;
    ThisFrameDigitalKeys.clear();

    PreviousCursorPosition = CursorPosition;
    ScrollDelta = 0;
}

void CInputManager::AddMappingContext(std::shared_ptr<SInputMappingContext> Context) {
    MappingContexts.push_back(Context);
}

void CInputManager::RemoveMappingContext(std::shared_ptr<SInputMappingContext> Context) {
    MappingContexts.erase(std::remove(MappingContexts.begin(), MappingContexts.end(), Context), MappingContexts.end());
}

void CInputManager::ProcessDigitalInput(EKeys Key, SInputAction Action) {
    auto callback = std::get<FDigitalAction>(Action.Callback);
    assert(callback && "Digital action callback is not set");

    int newState = glfwGetKey(GEngine->GetWindow(), KeyToGLFWCode.at(Key));
    EInputEvent state = EInputEvent::None;
    if (newState && !PreviousFrameDigitalKeys.contains(Key)) {
        state = EInputEvent::Pressed;
        ThisFrameDigitalKeys.insert(Key);
    } else if (newState && PreviousFrameDigitalKeys.contains(Key)) {
        state = EInputEvent::Triggered;
        ThisFrameDigitalKeys.insert(Key);
    } else if (!newState && PreviousFrameDigitalKeys.contains(Key)) {
        state = EInputEvent::Released;
    }

    switch (Action.InputEvent) {
    case EInputEvent::Pressed:
        if (state == EInputEvent::Pressed) {
            callback();
        }
        break;
    case EInputEvent::Triggered:
        if (state == EInputEvent::Triggered) {
            callback();
        }
        break;
    case EInputEvent::Released:
        if (state == EInputEvent::Released) {
            callback();
        }
        break;
    }
}

void CInputManager::ProcessAxis1DInput(EKeys Key, SInputAction Action) {
    auto callback = std::get<FAxis1DAction>(Action.Callback);
    assert(callback && "Axis1D action callback is not set");

    if (ScrollDelta == 0) return;

    if (Key == EKeys::MouseWheelAxis) {
        callback(ScrollDelta);
    }
}

void CInputManager::ProcessAxis2DInput(EKeys Key, SInputAction Action) {
    auto callback = std::get<FAxis2DAction>(Action.Callback);
    assert(callback && "Axis2D action callback is not set");

    const SVector2 delta = CursorPosition - PreviousCursorPosition;

    if (delta.X == 0 && delta.Y == 0) return;

    if (Key == EKeys::Mouse2D) {
        callback(delta);
    }
}

void CInputManager::OnScroll(float delta) { GInputManager->ScrollDelta = delta; }

void CInputManager::OnCursor(SVector2 delta) { GInputManager->CursorPosition = delta; }

void SInputMappingContext::AddMapping(EKeys Key, const SInputAction& Action) { KeyMappings[Key] = Action; }

void SInputMappingContext::RemoveMapping(EKeys Key) { KeyMappings.erase(Key); }
