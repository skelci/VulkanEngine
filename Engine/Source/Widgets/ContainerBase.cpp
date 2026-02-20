#include "ContainerBase.hpp"


std::vector<WWidget*> WContainerBase::GetChildren() const {
    std::vector<WWidget*> result;
    for (const auto& child : Children) {
        result.push_back(child.get());
        if (WContainerBase* container = Cast<WContainerBase>(child.get())) {
            result.append_range(container->GetChildren());
        }
    }
    return result;
}

int32 WContainerBase::GetChildIndex(const WWidget* Child) const {
    for (size_t i = 0; i < Children.size(); ++i) {
        if (Children[i].get() == Child) {
            return static_cast<int32>(i);
        }
    }
    return -1;
}
