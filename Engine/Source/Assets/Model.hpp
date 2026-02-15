#pragma once

#include "Mesh.hpp"

#include <memory>


class CModel : public CAssetBase {
public:
    std::vector<CMesh> Meshes;

    void SetMaterial(std::shared_ptr<CMaterial> InMaterial);

protected:
    virtual void LoadFromFile(const std::string& filepath) override;
};
