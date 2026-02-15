#pragma once

#include "AssetBase.hpp"

#include <string>
#include <vector>
#include <vulkan/vulkan.h>


class CShader : public CAssetBase {
public:
    ~CShader();

    VkShaderModule GetVertexShaderModule() const { return VertexShaderModule; }
    VkShaderModule GetFragmentShaderModule() const { return FragmentShaderModule; }

protected:
    void LoadFromFile(const std::string& FilePath) override;

private:
    VkShaderModule VertexShaderModule = VK_NULL_HANDLE;
    VkShaderModule FragmentShaderModule = VK_NULL_HANDLE;

    std::string ReadFileContent(const std::string& path);
    void SaveToFile(const std::string& path, const std::string& content);
    std::vector<char> ReadBinaryFile(const std::string& filename);
};
