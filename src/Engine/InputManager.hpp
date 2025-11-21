#pragma once

#include "glm/glm.hpp"

#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>


enum class EKeys : uint16_t {
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

enum class EInputValueType : uint8_t {
    Digital,
    Axis1D,
    Axis2D
};

enum class EInputEvent : uint8_t {
    Pressed,
    Triggered,
    Released,
    None
};

using FDigitalAction = std::function<void()>;
using FAxis1DAction = std::function<void(float)>;
using FAxis2DAction = std::function<void(glm::vec2)>;

using FInputCallback = std::variant<std::monostate, FDigitalAction, FAxis1DAction, FAxis2DAction>;

struct SInputAction {
    EInputValueType ValueType = EInputValueType::Digital;
    EInputEvent InputEvent = EInputEvent::Pressed;
    FInputCallback Callback = std::monostate{};

    SInputAction() = default;

    template <EInputValueType Type, typename Obj, typename MemFn, typename... Args>
    static SInputAction Create(EInputEvent evt, Obj* object, MemFn fn, Args&&... args) {
        SInputAction action;
        action.ValueType = Type;
        action.InputEvent = evt;

        auto tupleArgs = std::make_tuple(std::forward<Args>(args)...);

        if constexpr (Type == EInputValueType::Digital) {
            static_assert(std::is_invocable_v<MemFn, Obj*, Args...>,
                          "Digital callback must be callable as member(args...)");
            action.Callback = FDigitalAction([object, fn, tupleArgs = std::move(tupleArgs)]() mutable {
                std::apply([object, fn](auto&&... unpacked) {
                    std::invoke(fn, object, std::forward<decltype(unpacked)>(unpacked)...);
                },
                           tupleArgs);
            });
        } else if constexpr (Type == EInputValueType::Axis1D) {
            static_assert(std::is_invocable_v<MemFn, Obj*, float, Args...>,
                          "Axis1D callback must be callable as member(float, args...)");
            action.Callback = FAxis1DAction([object, fn, tupleArgs = std::move(tupleArgs)](float axis) mutable {
                std::apply([object, fn, axis](auto&&... unpacked) {
                    std::invoke(fn, object, axis, std::forward<decltype(unpacked)>(unpacked)...);
                },
                           tupleArgs);
            });
        } else {
            static_assert(std::is_invocable_v<MemFn, Obj*, glm::vec2, Args...>,
                          "Axis2D callback must be callable as member(vec2, args...)");
            action.Callback = FAxis2DAction([object, fn, tupleArgs = std::move(tupleArgs)](glm::vec2 axis) mutable {
                std::apply([object, fn, axis](auto&&... unpacked) {
                    std::invoke(fn, object, axis, std::forward<decltype(unpacked)>(unpacked)...);
                },
                           tupleArgs);
            });
        }

        return action;
    }
};

#define MAKE_INPUT_ACTION(ValueType, ...) \
    SInputAction::Create<ValueType>(__VA_ARGS__)


struct SInputMappingContext {
    uint16_t Priority = 0;

    void AddMapping(EKeys Key, const SInputAction& Action) {
        KeyMappings[Key] = Action;
    }

    void RemoveMapping(EKeys Key) {
        KeyMappings.erase(Key);
    }

    inline const std::unordered_map<EKeys, SInputAction>& GetMappings() const {
        return KeyMappings;
    }

private:
    std::unordered_map<EKeys, SInputAction> KeyMappings;
};


class CInputManager {
public:
    void Tick(float DeltaTime);

    void AddMappingContext(std::shared_ptr<SInputMappingContext> Context);
    void RemoveMappingContext(std::shared_ptr<SInputMappingContext> Context);

private:
    std::vector<std::shared_ptr<SInputMappingContext>> MappingContexts;

    void ProcessDigitalInput(EKeys Key, SInputAction Action);
    void ProcessAxis1DInput(EKeys Key, SInputAction Action);
    void ProcessAxis2DInput(EKeys Key, SInputAction Action);

    std::unordered_set<EKeys> ThisFrameDigitalKeys;
    std::unordered_set<EKeys> PreviousFrameDigitalKeys;
};
