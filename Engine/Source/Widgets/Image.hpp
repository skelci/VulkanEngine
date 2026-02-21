#pragma once

#include "Assets/Material.hpp"
#include "Widget.hpp"

#include <memory>

class CMaterial;


class WImage : public WWidget {
    CLASS_BODY(WImage, WWidget)

public:
    WImage();

    SVector2 Size = SVector2(50, 50);
    virtual SVector2 GetDesiredSize() const override { return Size; }

    void SetMaterial(std::shared_ptr<CMaterial> Material);

    template <typename T>
    void SetMaterialProperty(std::string name, const T& value) {
        GetMaterial()->SetProperty(name, value);
    }

private:
    CMaterial* GetMaterial() const;

    friend class CRenderer;
    std::shared_ptr<class CMesh> Mesh;
};
