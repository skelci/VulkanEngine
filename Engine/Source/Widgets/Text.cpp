#include "Text.hpp"
#include "EngineStatics.hpp"

#include <vector>


WText::WText() {
    Mesh = std::make_shared<CMesh>();
    Mesh->Material = GetAsset<CMaterial>("Engine/Materials/Text.mat");
    SetFont(GetAsset<CFont>("Engine/Fonts/arialbd.ttf"));
}

void WText::SetSize(const SVector2& InSize) {
    Super::SetSize(InSize);
    RebuildMesh();
}

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

void WText::RebuildMesh() {
    if (!Font || Text.empty()) {
        Mesh->SetData({}, {});
        return;
    }

    std::vector<SVertex> vertices;
    std::vector<uint32> indices;

    float x = 0.0f;
    float y = Font->Size;

    const float yScale = 2 / Font->Size;
    const float xScale = yScale / Size.X * Size.Y;

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
        vertices.push_back({{q.x0 * xScale, q.y0 * yScale, 0.0f}, {1, 1, 1}, {q.s0, q.t0}, {0, 0, 1}});
        // Vert 1: Bottom-Left
        vertices.push_back({{q.x0 * xScale, q.y1 * yScale, 0.0f}, {1, 1, 1}, {q.s0, q.t1}, {0, 0, 1}});
        // Vert 2: Bottom-Right
        vertices.push_back({{q.x1 * xScale, q.y1 * yScale, 0.0f}, {1, 1, 1}, {q.s1, q.t1}, {0, 0, 1}});
        // Vert 3: Top-Right
        vertices.push_back({{q.x1 * xScale, q.y0 * yScale, 0.0f}, {1, 1, 1}, {q.s1, q.t0}, {0, 0, 1}});
    }

    Mesh->SetData(vertices, indices);
}
