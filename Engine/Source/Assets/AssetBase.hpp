#pragma once

#include "Object.hpp"

#include <string>


class CAssetBase : public CObject {
    CLASS_BODY(CAssetBase, CObject)

protected:
    friend class CAssetManager;
    virtual void LoadFromFile(const std::string& FilePath) {}
};
