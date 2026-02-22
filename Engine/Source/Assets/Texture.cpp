#include "Texture.hpp"

#include "EngineStatics.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


CTexture::~CTexture() {
    CRenderer* Renderer = GEngine->GetRenderer();
    VkDevice device = Renderer->GetDevice();

    VkImageView imgView = ImageView;
    VkImage img = Image;
    VkDeviceMemory imgMem = ImageMemory;
    Renderer->EnqueueForDeletion([device, imgView, img, imgMem]() {
        vkDestroyImageView(device, imgView, nullptr);
        vkDestroyImage(device, img, nullptr);
        vkFreeMemory(device, imgMem, nullptr);
    });
}

void CTexture::CreateFromBuffer(void* pixels, int texWidth, int texHeight) {
    if (!pixels) {
        throw std::runtime_error("Found no pixels to create texture from!");
    }

    VkDeviceSize imageSize = texWidth * texHeight * 4;
    MipLevels = static_cast<uint32>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    CRenderer* Renderer = GEngine->GetRenderer();
    VkDevice device = Renderer->GetDevice();

    Renderer->CreateBuffer(
        imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory
    );

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device, stagingBufferMemory);

    Renderer->CreateImage(
        texWidth, texHeight, MipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, Image, ImageMemory
    );

    Renderer->TransitionImageLayout(
        Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, MipLevels
    );
    Renderer->CopyBufferToImage(
        stagingBuffer, Image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight)
    );

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

    Renderer->GenerateMipmaps(Image, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, MipLevels);

    ImageView = Renderer->CreateImageView(Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, MipLevels);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = Renderer->GetDescriptorPool();
    allocInfo.descriptorSetCount = 1;
    VkDescriptorSetLayout textureSetLayout = Renderer->GetTextureSetLayout();
    allocInfo.pSetLayouts = &textureSetLayout;

    if (vkAllocateDescriptorSets(device, &allocInfo, &DescriptorSet) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate texture descriptor set!");
    }

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = ImageView;
    imageInfo.sampler = Renderer->GetTextureSampler();

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = DescriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
}

void CTexture::LoadFromFile(const std::string& FilePath) {
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(FilePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    if (!pixels) {
        Log("Texture", ELogLevel::Error, "Failed to load texture image: " + FilePath);
        return;
    }

    CreateFromBuffer(pixels, texWidth, texHeight);

    stbi_image_free(pixels);
}
