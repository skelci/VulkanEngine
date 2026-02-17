#pragma once

#include "Widget.hpp"


class WImage : public WWidget {
    CLASS_BODY(WImage, WWidget)

public:
    WImage();

    void SetMaterial(std::shared_ptr<class CMaterial> Material);

private:
    friend class CRenderer;
    std::shared_ptr<class CMesh> Mesh;
};
