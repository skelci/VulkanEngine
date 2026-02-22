#pragma once

#include "Core/Delegate.hpp"

#include <array>
#include <string_view>


enum class ELogLevel { Debug, Verbose, Info, Warning, Error };

class CLog {
public:
    void Initialize();
    void Log(std::string_view category, ELogLevel level, std::string_view message);

    std::string LogPath = "Saved/Logs";
    TDelegate<ELogLevel /*LogLevel*/, std::string_view /*Message*/> OnLogMessage;

private:
    const std::array<std::string, 5> LogLevelNames = {"Debug", "Verbose", "Info", "Warning", "Error"};
    const std::array<std::string, 5> LogLevelAnsiColors = {
        "\033[36m", // Debug - Cyan
        "\033[90m", // Verbose - Gray
        "\033[0m",  // Info - Default
        "\033[33m", // Warning - Yellow
        "\033[31m"  // Error - Red
    };
    std::string FullLogPath;
};
