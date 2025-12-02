#pragma once

#include "AssetBase.hpp"

#include <memory>
#include <string>
#include <unordered_map>


class CAssetManager {
public:
    template <typename T>
    std::shared_ptr<T> GetAsset(const std::string& FilePath) {
        static_assert(std::is_base_of_v<CAssetBase, T>, "T must be derived from CAssetBase");

        auto it = AssetCache.find(FilePath);
        if (it != AssetCache.end()) {
            return std::static_pointer_cast<T>(it->second);
        }

        std::shared_ptr<T> NewAsset = std::make_shared<T>();
        static_cast<CAssetBase*>(NewAsset.get())->LoadFromFile(AssetRootPath + FilePath);
        AssetCache[FilePath] = NewAsset;
        return NewAsset;
    }

private:
    std::string AssetRootPath = "Content/";
    std::unordered_map<std::string, std::shared_ptr<CAssetBase>> AssetCache;
};
