#include "Widget.hpp"

#include "ContainerBase.hpp"
#include "EngineStatics.hpp"


SVector2 WWidget::GetDesiredSize() const { return SVector2(); }

SWidgetTransform WWidget::GetTransform() const {
    if (!Parent) {
        return SWidgetTransform{SVector2(), GEngine->GetRenderer()->GetScreenSize()};
    }
    return Parent->GetChildTransform(this);
}
