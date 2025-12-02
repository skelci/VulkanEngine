#pragma once

#include <string>

class CAssetManager;


class CAssetBase {
    friend class CAssetManager;

protected:
    virtual void LoadFromFile(const std::string& FilePath) = 0;
};
