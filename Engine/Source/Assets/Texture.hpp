#pragma once

#include "AssetBase.hpp"
#include "DataTypes.hpp"
#include <vulkan/vulkan.h>


class CTexture : public CAssetBase {
    CLASS_BODY(CTexture, CAssetBase)

public:
    ~CTexture();

    VkDescriptorSet GetDescriptorSet() const { return DescriptorSet; }
    VkImageView GetImageView() const { return ImageView; }

    void CreateFromBuffer(void* pixels, int texWidth, int texHeight);
    void LoadFromMemory(const unsigned char* buffer, int len);

    bool IsValid() const { return ImageView != VK_NULL_HANDLE; }

protected:
    void LoadFromFile(const std::string& FilePath) override;

private:
    friend class CRenderer;

    VkImage Image;
    VkDeviceMemory ImageMemory;
    VkImageView ImageView;

    VkDescriptorSet DescriptorSet;

    uint32 MipLevels;
};
