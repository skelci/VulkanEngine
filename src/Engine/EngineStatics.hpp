#pragma once

#include "Log.hpp"
#include "InputManager.hpp"
#include "Engine.hpp"

extern CEngine* GEngine;
extern CLog* GLog;
extern CInputManager* GInputManager;

inline void Log(const char* category, ELogLevel level, const std::string_view& message) {
    GLog->Log(category, level, message);
}
