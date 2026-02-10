#pragma once

#include "AssetBase.hpp"
#include "DataTypes.hpp"

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

        const int32 FirstSeparatorPos = FilePath.find_first_of("/\\");
        if (FirstSeparatorPos == std::string::npos) {
            throw std::runtime_error("Invalid file path: " + FilePath);
        }

        const std::string AssetPath = FilePath.substr(FirstSeparatorPos + 1);
        const std::string AssetType = FilePath.substr(0, FirstSeparatorPos);
        std::string FullAssetPath;
        if (AssetType == "Engine") {
            FullAssetPath = EngineAssetsRootPath + "/" + AssetPath;
        } else if (AssetType == "Game") {
            FullAssetPath = GameAssetsRootPath + "/" + AssetPath;
        } else {
            throw std::runtime_error("File path must start with 'Engine/' or 'Game/': " + FilePath);
        }

        std::shared_ptr<T> NewAsset = std::make_shared<T>();
        static_cast<CAssetBase*>(NewAsset.get())->LoadFromFile(FullAssetPath);
        AssetCache[FilePath] = NewAsset;
        return NewAsset;
    }

private:
    const std::string EngineAssetsRootPath = "Engine/Content";
    const std::string GameAssetsRootPath = "Game/Content";
    std::unordered_map<std::string, std::shared_ptr<CAssetBase>> AssetCache;
};
