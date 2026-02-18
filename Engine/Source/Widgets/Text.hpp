#pragma once

#include "Assets/Font.hpp"
#include "Assets/Material.hpp"
#include "Assets/Mesh.hpp"
#include "Widget.hpp"

#include <string>


class WText : public WWidget {
    CLASS_BODY(WText, WWidget)

public:
    WText();

    virtual void SetSize(const SVector2& InSize) override;

    void SetText(const std::string& InText);
    void SetFont(std::shared_ptr<CFont> InFont);

private:
    friend class CRenderer;
    std::shared_ptr<class CMesh> Mesh;

    std::string Text;
    std::shared_ptr<CFont> Font;

    void RebuildMesh();
};
