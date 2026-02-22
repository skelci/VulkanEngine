#pragma once

#include "EngineStatics.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

#include <memory>


class CModel : public CAssetBase {
    CLASS_BODY(CModel, CAssetBase)

public:
    std::vector<CMesh> Meshes;

    void SetMaterial(std::shared_ptr<CMaterial> InMaterial);
    void SetMaterialToMesh(size_t MeshIndex, std::shared_ptr<CMaterial> InMaterial);

    template <typename T>
    void SetMaterialProperty(const std::string& PropertyName, const T& Value);

    template <typename T>
    void SetMaterialPropertyToMesh(size_t MeshIndex, const std::string& PropertyName, const T& Value);

protected:
    virtual void LoadFromFile(const std::string& filepath) override;
};

template <typename T>
inline void CModel::SetMaterialProperty(const std::string& PropertyName, const T& Value) {
    for (auto& Mesh : Meshes) {
        Mesh.Material->SetProperty(PropertyName, Value);
    }
}

template <typename T>
inline void CModel::SetMaterialPropertyToMesh(size_t MeshIndex, const std::string& PropertyName, const T& Value) {
    if (MeshIndex < Meshes.size()) {
        Meshes[MeshIndex].Material->SetProperty(PropertyName, Value);
    } else {
        Log("Model", ELogLevel::Error,
            "Trying to set material property to mesh index " + std::to_string(MeshIndex) + " but model only has " +
                std::to_string(Meshes.size()) + " meshes.");
    }
}
