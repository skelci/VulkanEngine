#include "Model.hpp"

#include "AssetManager.hpp"
#include "EngineStatics.hpp"
#include "Material.hpp"
#include "Texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <filesystem>
#include <vector>


void CModel::SetMaterial(std::shared_ptr<CMaterial> InMaterial) {
    for (auto& Mesh : Meshes) {
        Mesh.Material = InMaterial;
    }
}

void CModel::SetMaterialToMesh(size_t MeshIndex, std::shared_ptr<CMaterial> InMaterial) {
    if (MeshIndex < Meshes.size()) {
        Meshes[MeshIndex].Material = InMaterial;
    } else {
        Log("Model", ELogLevel::Error,
            "Trying to set material to mesh index " + std::to_string(MeshIndex) + " but model only has " +
                std::to_string(Meshes.size()) + " meshes.");
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

    std::vector<std::shared_ptr<CMaterial>> materials;
    materials.reserve(scene->mNumMaterials);

    for (uint32_t i = 0; i < scene->mNumMaterials; ++i) {
        const aiMaterial* aiMat = scene->mMaterials[i];
        auto material = GetAsset<CMaterial>("Engine/Materials/SimpleShading.mat", true);

        Log("Model", ELogLevel::Verbose, " - Processing material " + std::to_string(i));

        // Diffuse Color
        aiColor3D color(1.f, 1.f, 1.f);
        if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
            Log("Model", ELogLevel::Verbose,
                "   - Diffuse color: " + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " +
                    std::to_string(color.b));
            material->SetProperty("Color", SVector4(color.r, color.g, color.b, 1.0f));
        }

        // Texture
        if (aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString str;
            aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
            std::string texturePath = str.C_Str();
            Log("Model", ELogLevel::Verbose, "   - Diffuse texture: " + texturePath);

            // Handle embedded textures if starts with *
            if (texturePath.length() > 0 && texturePath[0] == '*') {
                Log("Model", ELogLevel::Warning, " - Embedded textures not fully supported yet: " + texturePath);
            } else {
                std::filesystem::path fullPath = modelDir / texturePath;
                std::string fullPathStr = fullPath.string();
                std::replace(fullPathStr.begin(), fullPathStr.end(), '\\', '/');

                auto texture = GetAsset<CTexture>(fullPathStr);
                material->SetProperty("Texture", texture);
            }
        }

        // Roughness / Shininess
        float shininess = 0.0f;
        if (aiMat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
            Log("Model", ELogLevel::Verbose, "   - Shininess: " + std::to_string(shininess));

            // Map shininess (0-1000 usually) to roughness (0-1)
            // This is a rough approximation
            float roughness = 1.0f - std::clamp(shininess / 100.0f, 0.0f, 1.0f);
            material->SetProperty("Roughness", roughness);
        }

        // Roughness Map (using specular map if available)
        if (aiMat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
            aiString str;
            aiMat->GetTexture(aiTextureType_SPECULAR, 0, &str);
            std::string texturePath = str.C_Str();
            Log("Model", ELogLevel::Verbose, "   - Roughness texture: " + texturePath);

            // Handle embedded textures if starts with *
            if (texturePath.length() > 0 && texturePath[0] == '*') {
                Log("Model", ELogLevel::Warning, " - Embedded textures not fully supported yet: " + texturePath);
            } else {
                std::filesystem::path fullPath = modelDir / texturePath;
                std::string fullPathStr = fullPath.string();
                std::replace(fullPathStr.begin(), fullPathStr.end(), '\\', '/');

                auto texture = GetAsset<CTexture>(fullPathStr);
                material->SetProperty("RoughnessMap", texture);
            }
        }

        // Transparency / Opacity
        float opacity = 0.0f;
        if (aiMat->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS) {
            Log("Model", ELogLevel::Verbose, "   - Opacity: " + std::to_string(opacity));
            if (opacity >= 0.99f) {
                material->SetProperty("Translucent", 0.0f);
            }
        }

        materials.push_back(material);
    }

    Meshes.reserve(scene->mNumMeshes);

    for (uint32_t i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh* aiMesh = scene->mMeshes[i];
        CMesh mesh;

        if (aiMesh->mMaterialIndex < materials.size()) {
            mesh.Material = materials[aiMesh->mMaterialIndex];
            Log("Model", ELogLevel::Verbose,
                " - Assigning material " + std::to_string(aiMesh->mMaterialIndex) + " to mesh " + std::to_string(i));
        } else {
            Log("Model", ELogLevel::Warning,
                " - Mesh " + std::to_string(i) +
                    " references invalid material index: " + std::to_string(aiMesh->mMaterialIndex));
        }

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

            if (aiMesh->HasNormals()) {
                vertex.Normal = {aiMesh->mNormals[v].x, aiMesh->mNormals[v].y, aiMesh->mNormals[v].z};
            } else {
                vertex.Normal = {0.0f, 1.0f, 0.0f};
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
