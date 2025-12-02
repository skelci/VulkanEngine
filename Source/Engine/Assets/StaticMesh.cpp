#include "StaticMesh.hpp"
#include "EngineStatics.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


VkVertexInputBindingDescription SVertex::GetBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(SVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> SVertex::GetAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

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

    return attributeDescriptions;
}


CStaticMesh::~CStaticMesh() {
    VkDevice device = GEngine->GetRenderer()->GetDevice();

    vkDestroyBuffer(device, VertexBuffer, nullptr);
    vkFreeMemory(device, VertexBufferMemory, nullptr);
    vkDestroyBuffer(device, IndexBuffer, nullptr);
    vkFreeMemory(device, IndexBufferMemory, nullptr);
}

void CStaticMesh::LoadFromFile(const std::string& FilePath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        FilePath, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices
    );

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        std::runtime_error(std::string("Assimp: ") + importer.GetErrorString());
        Log("StaticMesh", ELogLevel::Error,
            "Failed to load mesh from file: " + FilePath + " with error: " + importer.GetErrorString());
        return;
    }

    const aiMesh* mesh = scene->mMeshes[0];
    Vertices.clear();
    Indices.clear();
    Vertices.reserve(mesh->mNumVertices);

    for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
        SVertex v{};
        v.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        v.Color = mesh->HasVertexColors(0)
                      ? glm::vec3(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b)
                      : glm::vec3(1.0);
        v.TexCoord = mesh->HasTextureCoords(0) ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                                               : glm::vec2(0.0);
        Vertices.push_back(v);
    }

    for (uint32_t f = 0; f < mesh->mNumFaces; ++f) {
        const aiFace& face = mesh->mFaces[f];
        Indices.insert(Indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
    }

    CRenderer* Renderer = GEngine->GetRenderer();
    Renderer->CreateBuffer(VertexBuffer, VertexBufferMemory, Vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    Renderer->CreateBuffer(IndexBuffer, IndexBufferMemory, Indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
}
