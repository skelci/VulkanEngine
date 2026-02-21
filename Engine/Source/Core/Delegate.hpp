#pragma once

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>


template <typename... Args>
class TDelegate {
public:
    void Bind(void (*fn)(Args...)) { Callbacks.emplace_back(std::make_unique<FreeBinding>(fn)); }

    template <typename T>
    void Bind(T* instance, void (T::*method)(Args...)) {
        Callbacks.emplace_back(std::make_unique<MemberBinding<T>>(instance, method));
    }

    template <typename Func>
    void Bind(Func&& f) {
        Callbacks.emplace_back(std::make_unique<LambdaBinding<std::decay_t<Func>>>(std::forward<Func>(f)));
    }

    void Unbind(void (*fn)(Args...)) {
        RemoveIf([fn](const BindingPtr& b) { return b->Matches(nullptr, &fn); });
    }

    template <typename T>
    void Unbind(T* instance, void (T::*method)(Args...)) {
        RemoveIf([instance, method](const BindingPtr& b) { return b->Matches(instance, &method); });
    }

    void Broadcast(Args... params) {
        for (auto& cb : Callbacks) {
            cb->Invoke(params...);
        }
    }

    void Clear() { Callbacks.clear(); }

private:
    struct IBinding {
        virtual ~IBinding() = default;
        virtual void Invoke(Args... args) = 0;
        virtual bool Matches(void* instance, const void* methodPtr) const = 0;
    };

    struct FreeBinding final : IBinding {
        using Func = void (*)(Args...);
        explicit FreeBinding(Func fn) : Fn(fn) {}
        void Invoke(Args... args) override { Fn(args...); }
        bool Matches(void* instance, const void* methodPtr) const override {
            if (instance != nullptr) return false;
            auto typed = *static_cast<const Func*>(methodPtr);
            return typed == Fn;
        }
        Func Fn;
    };

    template <typename T>
    struct MemberBinding final : IBinding {
        using Method = void (T::*)(Args...);
        MemberBinding(T* inst, Method method) : Instance(inst), MethodPtr(method) {}
        void Invoke(Args... args) override { (Instance->*MethodPtr)(args...); }
        bool Matches(void* instance, const void* methodPtr) const override {
            if (instance != Instance) return false;
            auto typed = *static_cast<const Method*>(methodPtr);
            return typed == MethodPtr;
        }
        T* Instance;
        Method MethodPtr;
    };

    template <typename Func>
    struct LambdaBinding final : IBinding {
        Func Fn;
        explicit LambdaBinding(Func fn) : Fn(std::move(fn)) {}
        void Invoke(Args... args) override { Fn(args...); }
        // Lambdas cannot be easily compared for equality, so they don't support targeted Unbind
        bool Matches(void* instance, const void* methodPtr) const override { return false; }
    };

    using BindingPtr = std::unique_ptr<IBinding>;
    std::vector<BindingPtr> Callbacks;

    template <typename Pred>
    void RemoveIf(Pred&& pred) {
        Callbacks.erase(std::remove_if(Callbacks.begin(), Callbacks.end(), std::forward<Pred>(pred)), Callbacks.end());
    }
};
