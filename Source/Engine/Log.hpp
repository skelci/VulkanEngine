#pragma once

#include "Core/Delegate.hpp"

#include <array>
#include <string_view>


enum class ELogLevel { Info, Warning, Error };

class CLog {
public:
    void Initialize();
    void Log(std::string_view category, ELogLevel level, std::string_view message);

    std::string LogPath = "Saves/Logs";
    TDelegate<ELogLevel /*LogLevel*/, std::string_view /*Message*/> OnLogMessage;

private:
    const std::array<std::string, 3> LogLevelNames = {"Info", "Warning", "Error"};
    const std::array<std::string, 3> LogLevelAnsiColors = {
        "\033[0m",  // Info - Default
        "\033[33m", // Warning - Yellow
        "\033[31m"  // Error - Red
    };
    std::string FullLogPath;
};
