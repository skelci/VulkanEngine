#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>


class CObject;

class CClass {
public:
    std::string Name;
    std::function<CObject*()> CreateInstance;
    CClass* SuperClass;

    bool IsChildOf(const CClass* Other) const;
};


template <typename T>
class TSubclassOf {
public:
    TSubclassOf() : ClassPtr(nullptr) {}

    // Allow assignment from a specific CClass*
    TSubclassOf(CClass* InClass) : ClassPtr(InClass) {
#ifndef NDEBUG
        if (InClass && !InClass->IsChildOf(T::StaticClass())) {
            throw std::invalid_argument("InClass must be a subclass of T");
        }
#endif // NDEBUG
    }

    // Allow assignment from TSubclassOf<Child>
    template <typename U>
    TSubclassOf(const TSubclassOf<U>& Other) : ClassPtr(Other.ClassPtr) {
        static_assert(std::is_base_of_v<T, U>, "U must derive from T");
    }

    T* CreateObject() const {
        if (ClassPtr) {
            return static_cast<T*>(ClassPtr->CreateInstance());
        }
        return nullptr;
    }

    CClass* Get() const { return ClassPtr; }
    CClass* operator->() const { return ClassPtr; }
    CClass* operator*() const { return ClassPtr; }

    operator bool() const { return ClassPtr != nullptr; }

private:
    CClass* ClassPtr;
};


class CObject {
public:
    CObject();
    virtual ~CObject();

    virtual CClass* GetClass() const { return StaticClass(); }

    // Check if this instance is of specific type
    template <typename T>
    bool IsA() const {
        return GetClass()->IsChildOf(T::StaticClass());
    }

    // Returns a weak ptr that expires when this object is destroyed.
    std::weak_ptr<bool> GetWeakPtr() const { return ValidityToken; }

    // Static helper to check if an object associated with a validity token is largely safe to access
    static bool IsValid(const std::weak_ptr<bool>& Token) { return !Token.expired(); }

    static CClass* StaticClass();

private:
    std::shared_ptr<bool> ValidityToken;
};


template <typename T>
T* Cast(CObject* Object) {
    if (Object && Object->IsA<T>()) {
        return static_cast<T*>(Object);
    }
    return nullptr;
}

template <typename T>
const T* Cast(const CObject* Object) {
    if (Object && Object->IsA<T>()) {
        return static_cast<const T*>(Object);
    }
    return nullptr;
}


#define CLASS_BODY(Class, BaseClass)                                                                                   \
public:                                                                                                                \
    using Super = BaseClass;                                                                                           \
    static CClass* StaticClass() {                                                                                     \
        static CClass ClassInstance = {#Class, []() -> CObject* { return new Class(); }, Class::Super::StaticClass()}; \
        return &ClassInstance;                                                                                         \
    }                                                                                                                  \
    virtual CClass* GetClass() const override { return Class::StaticClass(); }                                         \
                                                                                                                       \
private:
