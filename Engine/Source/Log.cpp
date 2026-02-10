#include "Log.hpp"

#include <cstdio>
#include <filesystem>
#include <fstream>


void CLog::Initialize() {
    FullLogPath = LogPath + "/logLatest.log";

    namespace fs = std::filesystem;
    const fs::path logDir{LogPath};
    fs::create_directories(logDir);

    const fs::path oldLog = logDir / "logLatest.log";
    if (fs::exists(oldLog)) {
        int idx = 1;
        fs::path rotated;
        do {
            rotated = logDir / ("log" + std::to_string(idx) + ".log");
            ++idx;
        } while (fs::exists(rotated));
        fs::rename(oldLog, rotated.parent_path() / ("log" + std::to_string(idx - 1) + ".log"));
    }
}

void CLog::Log(std::string_view category, ELogLevel level, std::string_view message) {
    const std::string_view& levelName = LogLevelNames[static_cast<std::size_t>(level)];
    const std::string_view& levelColor = LogLevelAnsiColors[static_cast<std::size_t>(level)];
    std::string logMessage;
    logMessage.reserve(1 + levelName.size() + 2 + category.size() + 2 + message.size() + 1);
    logMessage.append("[");
    logMessage.append(levelName);
    logMessage.append("][");
    logMessage.append(category);
    logMessage.append("] ");
    logMessage.append(message);
    logMessage.append("\n");

    printf("%s%s", levelColor.data(), logMessage.c_str());

    std::ofstream logFile;
    logFile.open(FullLogPath, std::ios::app);

    if (logFile.is_open()) {
        logFile << logMessage;
        logFile.close();
    }

    OnLogMessage.Broadcast(level, message);
}
