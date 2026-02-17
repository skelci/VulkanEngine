#include "Widget.hpp"


std::vector<WWidget*> WWidget::GetChildren() const {
    std::vector<WWidget*> childs;
    for (const auto& child : Children) {
        childs.push_back(child.get());
    }
    return childs;
}

std::vector<SWidgetWithTransform> WWidget::GetChildrensTransformed() const {
    std::vector<SWidgetWithTransform> result;

    SWidgetWithTransform self{Position, Size, const_cast<WWidget*>(this)};

    for (const auto& child : Children) {
        SWidgetWithTransform childWidget{child->Position + Position, child->Size, child.get()};
        result.push_back(childWidget);
        auto childTransformed = child->GetChildrensTransformed();
        result.insert(result.end(), childTransformed.begin(), childTransformed.end());
    }

    return result;
}
