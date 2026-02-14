#include "Model.hpp"

#include "AssetManager.hpp"
#include "EngineStatics.hpp"
#include "Texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>


void CModel::SetTexture(std::shared_ptr<CTexture> InTexture) {
    for (auto& Mesh : Meshes) {
        Mesh.Texture = InTexture;
    }
}

void CModel::LoadFromFile(const std::string& filepath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filepath, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices
    );

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        Log("Model", ELogLevel::Error,
            "Failed to load model from file: " + filepath + " with error: " + importer.GetErrorString());
        return;
    }

    std::filesystem::path modelPath(filepath);
    std::filesystem::path modelDir = modelPath.parent_path();

    Meshes.reserve(scene->mNumMeshes);

    for (uint32_t i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh* aiMesh = scene->mMeshes[i];
        CMesh mesh;

        // Process Vertices
        mesh.Vertices.reserve(aiMesh->mNumVertices);
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

            mesh.Vertices.push_back(vertex);
        }

        // Process Indices
        for (uint32_t f = 0; f < aiMesh->mNumFaces; ++f) {
            const aiFace& face = aiMesh->mFaces[f];
            mesh.Indices.insert(mesh.Indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
        }

        mesh.InitRenderResources();
        Meshes.push_back(std::move(mesh));
    }
}
