#include "Font.hpp"
#include "EngineStatics.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

#include <fstream>
#include <vector>


void CFont::LoadFromFile(const std::string& FilePath) {
    std::ifstream file(FilePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        Log("Font", ELogLevel::Error, "Failed to load font file: " + FilePath);
        throw std::runtime_error("Failed to open font file");
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> ttfBuffer(fileSize);
    if (!file.read((char*)ttfBuffer.data(), fileSize)) {
        throw std::runtime_error("Failed to read font file");
    }

    std::vector<unsigned char> tempBitmap(BitmapWidth * BitmapHeight);

    int result =
        stbtt_BakeFontBitmap(ttfBuffer.data(), 0, Size, tempBitmap.data(), BitmapWidth, BitmapHeight, 32, 96, CData);

    if (result <= 0) {
        Log("Font", ELogLevel::Error, "Not all characters fit in the font texture!");
    }

    std::vector<unsigned char> rgbaBitmap(BitmapWidth * BitmapHeight * 4);
    for (int i = 0; i < BitmapWidth * BitmapHeight; ++i) {
        unsigned char alpha = tempBitmap[i];
        rgbaBitmap[i * 4 + 0] = 255;   // R
        rgbaBitmap[i * 4 + 1] = 255;   // G
        rgbaBitmap[i * 4 + 2] = 255;   // B
        rgbaBitmap[i * 4 + 3] = alpha; // A
    }

    Texture = std::make_shared<CTexture>();
    Texture->CreateFromBuffer(rgbaBitmap.data(), BitmapWidth, BitmapHeight);
}
