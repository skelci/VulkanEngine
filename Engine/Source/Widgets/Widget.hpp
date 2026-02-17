#pragma once

#include "Object.hpp"
#include "Vector.hpp"

#include <memory>


struct SWidgetWithTransform {
    SVector2 Position;
    SVector2 Size;
    class WWidget* Widget;
};


class WWidget : public CObject {
    CLASS_BODY(WWidget, CObject)

public:
    SVector2 Position = SVector2();
    SVector2 Size = SVector2(100, 100);

    WWidget* GetParent() const { return Parent; }

    template <typename T>
    T* GetParent() const {
        return Cast<T>(Parent);
    }

    template <typename T>
    T* AddChild() {
        static_assert(std::is_base_of_v<WWidget, T>, "T must be a subclass of WWidget");
        std::unique_ptr<T> widget = std::make_unique<T>();
        widget->Parent = this;
        T* ptr = widget.get();
        Children.push_back(std::move(widget));
        return ptr;
    }

    std::vector<WWidget*> GetChildren() const;

    template <typename T>
    std::vector<T*> GetChildren() const {
        std::vector<T*> result;
        for (const auto& child : Children) {
            if (T* casted = Cast<T>(child)) {
                result.push_back(casted);
            }
        }
        return result;
    }

    virtual std::vector<SWidgetWithTransform> GetChildrensTransformed() const;

private:
    WWidget* Parent = nullptr;
    std::vector<std::unique_ptr<WWidget>> Children;
};
