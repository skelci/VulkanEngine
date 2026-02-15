#pragma once

#include "AssetBase.hpp"

#include <vulkan/vulkan.h>

#include <string>
#include <unordered_map>
#include <vector>


enum class EShaderPropertyType { Float, Vec2, Vec3, Vec4, Texture };

struct SShaderProperty {
    std::string Name;
    EShaderPropertyType Type;
    size_t Offset;
    size_t Size;
    uint32_t Binding;

    // For UBO members, this is the byte offset.
    // For Textures, this is the binding index (starting after UBO).
};

class CShader : public CAssetBase {
public:
    ~CShader();

    VkShaderModule GetVertexShaderModule() const { return VertexShaderModule; }
    VkShaderModule GetFragmentShaderModule() const { return FragmentShaderModule; }

    const std::vector<SShaderProperty>& GetProperties() const { return Properties; }
    const std::unordered_map<std::string, std::string>& GetDefaultValues() const { return DefaultValues; }
    size_t GetUBOSize() const { return UBOSize; }

protected:
    void LoadFromFile(const std::string& FilePath) override;

private:
    VkShaderModule VertexShaderModule = VK_NULL_HANDLE;
    VkShaderModule FragmentShaderModule = VK_NULL_HANDLE;

    std::vector<SShaderProperty> Properties;
    std::unordered_map<std::string, std::string> DefaultValues;
    size_t UBOSize = 0;

    std::string ReadFileContent(const std::string& path);
    void SaveToFile(const std::string& path, const std::string& content);
    std::vector<char> ReadBinaryFile(const std::string& filename);
};
