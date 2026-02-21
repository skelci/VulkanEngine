#pragma once

#include "DataTypes.hpp"
#include "Delegate.hpp"
#include "Vector.hpp"

#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>


enum class EKeys : uint8 {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Escape,
    LeftControl,
    LeftShift,
    LeftAlt,
    RightControl,
    RightShift,
    RightAlt,
    Space,
    Enter,
    Backspace,
    Tab,
    Mouse2D,
    MouseLeft,
    MouseRight,
    MouseMiddle,
    MouseWheelAxis
};

enum class EInputValueType : uint8_t { Digital, Axis1D, Axis2D };

enum class EInputEvent : uint8_t { Pressed, Triggered, Released, None };

using FDigitalAction = std::function<void()>;
using FAxis1DAction = std::function<void(float)>;
using FAxis2DAction = std::function<void(SVector2)>;

using FInputCallback = std::variant<FDigitalAction, FAxis1DAction, FAxis2DAction>;

struct SInputAction {
    EInputValueType ValueType;
    EInputEvent InputEvent;
    FInputCallback Callback;
    std::weak_ptr<bool> ValidityToken;

    bool IsValid() const { return !ValidityToken.expired(); }

    template <EInputValueType Type, typename Obj, typename MemFn, typename... Args>
    static SInputAction Create(EInputEvent evt, Obj* object, MemFn fn, Args&&... args) {
        SInputAction action;
        action.ValueType = Type;
        action.InputEvent = evt;
        action.ValidityToken = object->GetWeakPtr();

        auto tupleArgs = std::make_tuple(std::forward<Args>(args)...);

        if constexpr (Type == EInputValueType::Digital) {
            static_assert(
                std::is_invocable_v<MemFn, Obj*, Args...>, "Digital callback must be callable as member(args...)"
            );
            action.Callback = FDigitalAction([object, fn, tupleArgs = std::move(tupleArgs)]() mutable {
                std::apply(
                    [object, fn](auto&&... unpacked) {
                        std::invoke(fn, object, std::forward<decltype(unpacked)>(unpacked)...);
                    },
                    tupleArgs
                );
            });
        } else if constexpr (Type == EInputValueType::Axis1D) {
            static_assert(
                std::is_invocable_v<MemFn, Obj*, float, Args...>,
                "Axis1D callback must be callable as member(float, args...)"
            );
            action.Callback = FAxis1DAction([object, fn, tupleArgs = std::move(tupleArgs)](float axis) mutable {
                std::apply(
                    [object, fn, axis](auto&&... unpacked) {
                        std::invoke(fn, object, axis, std::forward<decltype(unpacked)>(unpacked)...);
                    },
                    tupleArgs
                );
            });
        } else {
            static_assert(
                std::is_invocable_v<MemFn, Obj*, SVector2, Args...>,
                "Axis2D callback must be callable as member(vec2, args...)"
            );
            action.Callback = FAxis2DAction([object, fn, tupleArgs = std::move(tupleArgs)](SVector2 axis) mutable {
                std::apply(
                    [object, fn, axis](auto&&... unpacked) {
                        std::invoke(fn, object, axis, std::forward<decltype(unpacked)>(unpacked)...);
                    },
                    tupleArgs
                );
            });
        }

        return action;
    }
};

#define MAKE_INPUT_ACTION(ValueType, ...) SInputAction::Create<ValueType>(__VA_ARGS__)


struct SInputMappingContext {
    uint16_t Priority = 0;

    void AddMapping(EKeys Key, const SInputAction& Action);

    void RemoveMapping(EKeys Key);

    const std::unordered_map<EKeys, std::vector<SInputAction>>& GetMappings() const { return KeyMappings; }

private:
    std::unordered_map<EKeys, std::vector<SInputAction>> KeyMappings;
};


class CInputManager {
public:
    CInputManager();

    void Tick(float DeltaTime);

    void AddMappingContext(SInputMappingContext* Context);
    void RemoveMappingContext(SInputMappingContext* Context);

    SVector2 GetCursorPosition() const { return CursorPosition; }
    TDelegate<char> OnCharInput;

    void SetInputMode(bool bCursorVisible);

private:
    std::vector<std::unique_ptr<SInputMappingContext>> MappingContexts;

    void ProcessDigitalInput(EKeys Key, SInputAction Action);
    void ProcessAxis1DInput(EKeys Key, SInputAction Action);
    void ProcessAxis2DInput(EKeys Key, SInputAction Action);

    std::unordered_set<EKeys> ThisFrameDigitalKeys;
    std::unordered_set<EKeys> PreviousFrameDigitalKeys;

    static void OnScroll(float delta);
    static void OnCursor(SVector2 delta);
    static void OnChar(unsigned int codepoint);
    static void OnKey(int key, int scancode, int action, int mods);

    float ScrollDelta = 0;
    SVector2 CursorPosition = SVector2();
    SVector2 PreviousCursorPosition = SVector2();
};
