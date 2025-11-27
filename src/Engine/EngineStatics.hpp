#pragma once

#include "Engine.hpp"
#include "InputManager.hpp"
#include "Log.hpp"

extern CEngine* GEngine;
extern CLog* GLog;
extern CInputManager* GInputManager;

inline void Log(const char* category, ELogLevel level, const std::string_view& message) {
    GLog->Log(category, level, message);
}
