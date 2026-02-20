#pragma once

#include "Object.hpp"
#include "Vector.hpp"

class WContainerBase;


struct SWidgetTransform {
    SVector2 Position;
    SVector2 Size;
};


class WWidget : public CObject {
    CLASS_BODY(WWidget, CObject)

public:
    virtual SVector2 GetDesiredSize() const;
    SWidgetTransform GetTransform() const;

    WContainerBase* GetParent() const { return Parent; }

    template <typename T>
    T* GetParent() const {
        return Cast<T>(Parent);
    }

private:
    friend WContainerBase;
    WContainerBase* Parent = nullptr;
};
