#pragma once

#include "AssetBase.hpp"
#include "Texture.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <vulkan/vulkan.h>


struct SVertex {
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoord;

    static VkVertexInputBindingDescription GetBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
};

class CMesh {
public:
    CMesh() = default;
    ~CMesh();

    // Move-only class to handle Vulkan resources safely
    CMesh(const CMesh&) = delete;
    CMesh& operator=(const CMesh&) = delete;
    CMesh(CMesh&& other) noexcept;
    CMesh& operator=(CMesh&& other) noexcept;

    const std::vector<SVertex>& GetVertices() const { return Vertices; }
    const std::vector<uint32_t>& GetIndices() const { return Indices; }

    const VkBuffer& GetVertexBuffer() const { return VertexBuffer; }
    const VkBuffer& GetIndexBuffer() const { return IndexBuffer; }

    void InitRenderResources();

    std::vector<SVertex> Vertices;
    std::vector<uint32_t> Indices;
    std::shared_ptr<CTexture> Texture;

private:
    VkBuffer VertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory VertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer IndexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory IndexBufferMemory = VK_NULL_HANDLE;

    void Cleanup();
};
