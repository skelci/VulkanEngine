#include "Mesh.hpp"

#include "EngineStatics.hpp"


VkVertexInputBindingDescription SVertex::GetBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(SVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 4> SVertex::GetAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(SVertex, Position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(SVertex, Color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(SVertex, TexCoord);

    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[3].offset = offsetof(SVertex, Normal);

    return attributeDescriptions;
}


CMesh::~CMesh() { Cleanup(); }

CMesh::CMesh(CMesh&& other) noexcept
    : Vertices(std::move(other.Vertices)), Indices(std::move(other.Indices)), Material(std::move(other.Material)),
      VertexBuffer(other.VertexBuffer), VertexBufferMemory(other.VertexBufferMemory), IndexBuffer(other.IndexBuffer),
      IndexBufferMemory(other.IndexBufferMemory) {
    other.VertexBuffer = VK_NULL_HANDLE;
    other.VertexBufferMemory = VK_NULL_HANDLE;
    other.IndexBuffer = VK_NULL_HANDLE;
    other.IndexBufferMemory = VK_NULL_HANDLE;
}

CMesh& CMesh::operator=(CMesh&& other) noexcept {
    if (this != &other) {
        Cleanup();

        Vertices = std::move(other.Vertices);
        Indices = std::move(other.Indices);
        Material = std::move(other.Material);
        VertexBuffer = other.VertexBuffer;
        VertexBufferMemory = other.VertexBufferMemory;
        IndexBuffer = other.IndexBuffer;
        IndexBufferMemory = other.IndexBufferMemory;

        other.VertexBuffer = VK_NULL_HANDLE;
        other.VertexBufferMemory = VK_NULL_HANDLE;
        other.IndexBuffer = VK_NULL_HANDLE;
        other.IndexBufferMemory = VK_NULL_HANDLE;
    }
    return *this;
}

void CMesh::Cleanup() {
    VkDevice device = GEngine->GetRenderer()->GetDevice();

    if (VertexBuffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(device, VertexBuffer, nullptr);
        VertexBuffer = VK_NULL_HANDLE;
    }
    if (VertexBufferMemory != VK_NULL_HANDLE) {
        vkFreeMemory(device, VertexBufferMemory, nullptr);
        VertexBufferMemory = VK_NULL_HANDLE;
    }
    if (IndexBuffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(device, IndexBuffer, nullptr);
        IndexBuffer = VK_NULL_HANDLE;
    }
    if (IndexBufferMemory != VK_NULL_HANDLE) {
        vkFreeMemory(device, IndexBufferMemory, nullptr);
        IndexBufferMemory = VK_NULL_HANDLE;
    }
}

void CMesh::InitRenderResources() {
    CRenderer* Renderer = GEngine->GetRenderer();
    Renderer->CreateBuffer(VertexBuffer, VertexBufferMemory, Vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    Renderer->CreateBuffer(IndexBuffer, IndexBufferMemory, Indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
}
