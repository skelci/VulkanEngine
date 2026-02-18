#include "Mesh.hpp"

#include "EngineStatics.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <filesystem>


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


void CMesh::LoadFromFile(const std::string& FilePath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        FilePath, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices
    );

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        Log("Mesh", ELogLevel::Error,
            "Failed to load model from file: " + FilePath + " with error: " + importer.GetErrorString());
        return;
    }

    std::filesystem::path modelPath(FilePath);
    std::filesystem::path modelDir = modelPath.parent_path();

    const aiMesh* aiMesh = scene->mMeshes[0];

    // Process Vertices
    Vertices.reserve(aiMesh->mNumVertices);
    for (uint32_t v = 0; v < aiMesh->mNumVertices; ++v) {
        SVertex vertex{};
        vertex.Position = {aiMesh->mVertices[v].x, aiMesh->mVertices[v].y, aiMesh->mVertices[v].z};

        if (aiMesh->HasVertexColors(0)) {
            vertex.Color = {aiMesh->mColors[0][v].r, aiMesh->mColors[0][v].g, aiMesh->mColors[0][v].b};
        } else {
            vertex.Color = {1.0f, 1.0f, 1.0f};
        }

        if (aiMesh->HasTextureCoords(0)) {
            vertex.TexCoord = {aiMesh->mTextureCoords[0][v].x, aiMesh->mTextureCoords[0][v].y};
        } else {
            vertex.TexCoord = {0.0f, 0.0f};
        }

        if (aiMesh->HasNormals()) {
            vertex.Normal = {aiMesh->mNormals[v].x, aiMesh->mNormals[v].y, aiMesh->mNormals[v].z};
        } else {
            vertex.Normal = {0.0f, 1.0f, 0.0f};
        }

        Vertices.push_back(vertex);
    }

    // Process Indices
    for (uint32_t f = 0; f < aiMesh->mNumFaces; ++f) {
        const aiFace& face = aiMesh->mFaces[f];
        Indices.insert(Indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
    }

    InitRenderResources();
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

void CMesh::SetData(const std::vector<SVertex>& InVertices, const std::vector<uint32_t>& InIndices) {
    Cleanup();
    Vertices = InVertices;
    Indices = InIndices;
    InitRenderResources();
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
    if (Vertices.empty() || Indices.empty()) return;
    CRenderer* Renderer = GEngine->GetRenderer();
    Renderer->CreateBuffer(VertexBuffer, VertexBufferMemory, Vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    Renderer->CreateBuffer(IndexBuffer, IndexBufferMemory, Indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
}
