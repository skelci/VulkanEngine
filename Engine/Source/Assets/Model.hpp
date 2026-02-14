#pragma once

#include "Mesh.hpp"

#include <memory>


class CModel : public CAssetBase {
public:
    std::vector<CMesh> Meshes;

    void SetTexture(std::shared_ptr<CTexture> InTexture);

protected:
    virtual void LoadFromFile(const std::string& filepath) override;
};
