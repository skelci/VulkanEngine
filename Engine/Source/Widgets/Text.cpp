#include "Text.hpp"

#include "Assets/Font.hpp"
#include "Assets/Material.hpp"
#include "Assets/Mesh.hpp"
#include "EngineStatics.hpp"

#include <vector>


WText::WText() {
    Mesh = std::make_shared<CMesh>();
    Mesh->Material = GetAsset<CMaterial>("Engine/Materials/Text.mat", true);
    SetFont(GetAsset<CFont>("Engine/Fonts/arialbd.ttf"));
}

SVector2 WText::GetDesiredSize() const { return SVector2(CachedTextWidth, FontSize); }

void WText::SetText(const std::string& InText) {
    if (Text != InText) {
        Text = InText;
        RebuildMesh();
    }
}

void WText::SetFont(std::shared_ptr<CFont> InFont) {
    Font = InFont;
    if (Font && Font->Texture) {
        Mesh->Material->SetProperty("Texture", Font->Texture);
    }
    RebuildMesh();
}

void WText::SetMaterial(std::shared_ptr<CMaterial> Material) {
    Mesh->Material = Material;
    RebuildMesh();
}

CMaterial* WText::GetMaterial() const { return Mesh->Material.get(); }

void WText::RebuildMesh() {
    if (!Font || Text.empty()) {
        Mesh->SetData({}, {});
        return;
    }

    std::vector<SVertex> vertices;
    std::vector<uint32> indices;

    float x = 0.0f;
    float y = FontSize * 0.5f;

    const float Scale = FontSize / Font->Size;

    for (char c : Text) {
        if (c < 32 || c >= 128) continue; // Skip unsupported chars

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(Font->CData, Font->BitmapWidth, Font->BitmapHeight, c - 32, &x, &y, &q, 1);

        uint32 vCount = static_cast<uint32>(vertices.size());
        indices.push_back(vCount + 2);
        indices.push_back(vCount + 1);
        indices.push_back(vCount + 0);
        indices.push_back(vCount + 0);
        indices.push_back(vCount + 3);
        indices.push_back(vCount + 2);

        // Vert 0: Top-Left
        vertices.push_back({{q.x0 * Scale, q.y0 * Scale, 0.0f}, {1, 1, 1}, {q.s0, q.t0}, {0, 0, 1}});
        // Vert 1: Bottom-Left
        vertices.push_back({{q.x0 * Scale, q.y1 * Scale, 0.0f}, {1, 1, 1}, {q.s0, q.t1}, {0, 0, 1}});
        // Vert 2: Bottom-Right
        vertices.push_back({{q.x1 * Scale, q.y1 * Scale, 0.0f}, {1, 1, 1}, {q.s1, q.t1}, {0, 0, 1}});
        // Vert 3: Top-Right
        vertices.push_back({{q.x1 * Scale, q.y0 * Scale, 0.0f}, {1, 1, 1}, {q.s1, q.t0}, {0, 0, 1}});
    }

    CachedTextWidth = x * Scale;

    Mesh->SetData(vertices, indices);
}
