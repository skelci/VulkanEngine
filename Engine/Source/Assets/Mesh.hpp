#pragma once

#include "AssetBase.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <vulkan/vulkan.h>

class CMaterial;


struct SVertex {
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoord;
    glm::vec3 Normal;

    static VkVertexInputBindingDescription GetBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions();
};

class CMesh : public CAssetBase {
    CLASS_BODY(CMesh, CAssetBase)

protected:
    void LoadFromFile(const std::string& FilePath) override;

public:
    CMesh() = default;
    ~CMesh();

    CMesh(const CMesh&) = delete;
    CMesh& operator=(const CMesh&) = delete;
    CMesh(CMesh&& other) noexcept;
    CMesh& operator=(CMesh&& other) noexcept;

    bool IsValid() const { return !Vertices.empty() && !Indices.empty(); }

    const std::vector<SVertex>& GetVertices() const { return Vertices; }
    const std::vector<uint32_t>& GetIndices() const { return Indices; }

    void SetData(const std::vector<SVertex>& InVertices, const std::vector<uint32_t>& InIndices);
    static CMesh CreateFromData(const std::vector<SVertex>& InVertices, const std::vector<uint32_t>& InIndices);

    const VkBuffer& GetVertexBuffer() const { return VertexBuffer; }
    const VkBuffer& GetIndexBuffer() const { return IndexBuffer; }

    void InitRenderResources();

    std::vector<SVertex> Vertices;
    std::vector<uint32_t> Indices;
    std::shared_ptr<CMaterial> Material;

private:
    VkBuffer VertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory VertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer IndexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory IndexBufferMemory = VK_NULL_HANDLE;

    void Cleanup();
};
