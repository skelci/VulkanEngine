#pragma once

#include "Core/Delegate.hpp"

#include <string_view>


enum class ELogLevel {
    Info,
    Warning,
    Error
};

class CLog {
public:
    static void Init();
    static void Log(std::string_view category, ELogLevel level, std::string_view message);

    static std::string LogPath;
    static TDelegate<std::string_view> OnLogMessage;

private:
    static const std::string LogLevelNames[];
    static const std::string LogLevelAnsiColors[];
    static std::string FullLogPath;
};
