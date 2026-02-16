#pragma once

#include "Object.hpp"
#include "Vector.hpp"

#include <memory>


class WWidget : public CObject {
    CLASS_BODY(WWidget, CObject)

public:
    WWidget();

    SVector2 Position = SVector2();
    SVector2 Size = SVector2(100, 100);

    void SetMaterial(std::shared_ptr<class CMaterial> Material);

private:
    friend class CRenderer;
    std::shared_ptr<class CMesh> Mesh;
};
