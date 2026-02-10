#pragma once

#include "AssetBase.hpp"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>


struct SVertex {
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoord;

    static VkVertexInputBindingDescription GetBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
};

class CStaticMesh : public CAssetBase {
public:
    ~CStaticMesh();

    const std::vector<SVertex>& GetVertices() const { return Vertices; }
    const std::vector<uint32_t>& GetIndices() const { return Indices; }

    const VkBuffer& GetVertexBuffer() const { return VertexBuffer; }
    const VkBuffer& GetIndexBuffer() const { return IndexBuffer; }

protected:
    void LoadFromFile(const std::string& FilePath) override;

private:
    std::vector<SVertex> Vertices;
    std::vector<uint32_t> Indices;

    VkBuffer VertexBuffer;
    VkDeviceMemory VertexBufferMemory;
    VkBuffer IndexBuffer;
    VkDeviceMemory IndexBufferMemory;
};
