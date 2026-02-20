#pragma once

#include "Assets/Material.hpp"
#include "Widget.hpp"

#include <memory>
#include <string>

class CFont;


class WText : public WWidget {
    CLASS_BODY(WText, WWidget)

public:
    WText();

    virtual SVector2 GetDesiredSize() const override;

    float FontSize = 24.0f;

    void SetText(const std::string& InText);
    void SetFont(std::shared_ptr<CFont> InFont);

    void SetMaterial(std::shared_ptr<CMaterial> Material);

    template <typename T>
    void SetMaterialProperty(std::string name, const T& value) {
        GetMaterial()->SetProperty(name, value);
    }

protected:
    CMaterial* GetMaterial() const;

    friend class CRenderer;
    std::shared_ptr<class CMesh> Mesh;

    std::string Text;
    std::shared_ptr<CFont> Font;
    float CachedTextWidth = 0.0f;

    void RebuildMesh();
};
