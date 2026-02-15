#pragma once

#include "AssetBase.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <memory>
#include <string>
#include <vulkan/vulkan.h>


class CMaterial : public CAssetBase {
public:
    ~CMaterial();

    VkPipeline GetPipeline() const { return Pipeline; }
    VkPipelineLayout GetPipelineLayout() const { return PipelineLayout; }
    std::shared_ptr<CTexture> GetTexture() const { return Texture; }

    bool IsValid() const { return Pipeline != VK_NULL_HANDLE; }

protected:
    void LoadFromFile(const std::string& FilePath) override;

private:
    std::shared_ptr<CShader> Shader;
    std::shared_ptr<CTexture> Texture;

    VkPipeline Pipeline = VK_NULL_HANDLE;
    VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;

    void CreatePipeline();
};
