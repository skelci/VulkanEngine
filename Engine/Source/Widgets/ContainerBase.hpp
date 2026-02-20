#pragma once

#include "DataTypes.hpp"
#include "Widget.hpp"

#include <memory>
#include <vector>


class WContainerBase : public WWidget {
    CLASS_BODY(WContainerBase, WWidget)

public:
    template <typename T>
    T* AddChild() {
        static_assert(std::is_base_of_v<WWidget, T>, "T must be a subclass of WWidget");
        std::unique_ptr<T> widget = std::make_unique<T>();
        widget->Parent = this;
        T* ptr = widget.get();
        Children.push_back(std::move(widget));
        OnChildAdded(ptr);
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

    virtual SWidgetTransform GetChildTransform(const WWidget* Child) const { return SWidgetTransform(); };
    int32 GetChildIndex(const WWidget* Child) const;

protected:
    virtual void OnChildAdded(WWidget* Widget) {}

    std::vector<std::unique_ptr<WWidget>> Children;
};
