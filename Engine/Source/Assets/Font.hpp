#pragma once

#include "AssetBase.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

#include <map>
#include <stb/stb_truetype.h>
#include <vector>


class CFont : public CAssetBase {
public:
    std::shared_ptr<CTexture> Texture;
    stbtt_bakedchar CData[96]; // ASCII 32..126 is 95 glyphs

    const float Size = 72.0f;
    const int BitmapWidth = 512;
    const int BitmapHeight = 512;

protected:
    void LoadFromFile(const std::string& FilePath) override;
};
