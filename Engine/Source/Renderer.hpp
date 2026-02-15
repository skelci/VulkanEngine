#pragma once

#include "Delegate.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <array>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class ACamera;


struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool IsComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct SUniformBufferObject {
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};


class CRenderer {
public:
    CRenderer();
    ~CRenderer();

    void Tick(float DeltaTime);

    void inline SetActiveCamera(ACamera* Camera) { ActiveCamera = Camera; }

    // Handles buffer creation with staging buffer internally
    template <typename T>
    void CreateBuffer(
        VkBuffer& buffer, VkDeviceMemory& bufferMemory, const std::vector<T>& data, VkBufferUsageFlags usage
    );

    void CreateBuffer(
        VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
        VkDeviceMemory& bufferMemory
    );

    void CreateImage(
        uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
        VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
        VkDeviceMemory& imageMemory
    );
    void TransitionImageLayout(
        VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels
    );
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
    void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

    VkDevice GetDevice() const { return device; }

    VkDescriptorSetLayout GetTextureSetLayout() const { return textureSetLayout; }
    VkSampler GetTextureSampler() const { return textureSampler; }
    VkDescriptorPool GetDescriptorPool() const { return descriptorPool; }

    void WaitForIdle() { vkDeviceWaitIdle(device); }

private:
    static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
    void InitVulkan();
    void CreateInstance();
    void SetupDebugMessenger();
    void CreateSurface();
    void CreateSwapChain();
    void RecreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateDescriptorSetLayout();
    void CreateGraphicsPipeline();
    void CreateFramebuffers();
    void CreateCommandPool();
    void CreateColorResources();
    void CreateDepthResources();
    void CreateTextureSampler();
    void CreateUniformBuffers();
    void CreateDescriptorSets();
    void CreateDescriptorPool();
    void CreateCommandBuffers();
    void CreateSyncObjects();

    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    VkFormat FindSupportedFormat(
        const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features
    );
    VkFormat FindDepthFormat();

    void PickPhysicalDevice();
    int RateDeviceSuitability(VkPhysicalDevice device);
    void CreateLogicalDevice();

    void DrawFrame();
    void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, uint32_t frameIndex);
    void UpdateUniformBuffer(uint32_t frameIndex);

    void Cleanup();
    void CleanupSwapChain();

    bool CheckValidationLayerSupport();
    std::vector<const char*> GetRequiredExtensions();

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR ChoseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChoseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChoseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData
    );

    static VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger
    );

    static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    static void DestroyDebugUtilsMessengerEXT(
        VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator
    );

    ACamera* ActiveCamera;

    GLFWwindow* window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSurfaceKHR surface;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;

    VkDescriptorSetLayout globalSetLayout;
    VkDescriptorSetLayout textureSetLayout;

    friend class CEngine;
    std::shared_ptr<class CTexture> DefaultTexture;
    std::shared_ptr<class CMaterial> DefaultMaterial;

public:
    std::shared_ptr<class CTexture> GetDefaultTexture() const { return DefaultTexture; }

private:
    VkPipelineLayout pipelineLayout;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    uint32_t currentFrame = 0;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> presentationSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> inFlightImages;

    bool framebufferResized = false;

    VkDebugUtilsMessengerEXT debugMessenger;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    VkSampler textureSampler;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

    VkSampleCountFlagBits GetMaxUsableSampleCount();

public:
    VkRenderPass GetRenderPass() const { return renderPass; }
    VkDescriptorSetLayout GetGlobalSetLayout() const { return globalSetLayout; }
    VkSampleCountFlagBits GetMSAASamples() const { return msaaSamples; }

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

private:
    const uint8_t MAX_FRAMES_IN_FLIGHT = 2;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    const bool AllowDedicatedGPUs = false;

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
};


template <typename T>
inline void CRenderer::CreateBuffer(
    VkBuffer& buffer, VkDeviceMemory& bufferMemory, const std::vector<T>& data, VkBufferUsageFlags usage
) {
    assert(!data.empty() && "Data array is empty, cannot create buffer!");

    VkDeviceSize bufferSize = sizeof(T) * data.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(
        bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory
    );

    void* dataPtr;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &dataPtr);
    memcpy(dataPtr, data.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    CreateBuffer(
        bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory
    );

    CopyBuffer(stagingBuffer, buffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}
