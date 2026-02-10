#pragma once

#include "Assets/AssetManager.hpp"
#include "Engine.hpp"
#include "InputManager.hpp"
#include "Log.hpp"

extern CEngine* GEngine;
extern CLog* GLog;
extern CInputManager* GInputManager;
extern CAssetManager* GAssetManager;

inline void Log(const char* category, ELogLevel level, const std::string_view& message) {
    GLog->Log(category, level, message);
}

template <typename T>
inline std::shared_ptr<T> GetAsset(const std::string& filePath) {
    return GAssetManager->GetAsset<T>(filePath);
}
