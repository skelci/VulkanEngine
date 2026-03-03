#pragma once

#include "CoreMinimal.hpp"


struct SLeaderboardEntry {
    int32 Level = 0;
    int32 TotalScore = 0;
};

class CGameInstance : public CObject {
    CLASS_BODY(CGameInstance, CObject)

public:
    std::string Username = "Player";
    int32 Level = 1;
    int32 TotalScore = 0;

    std::map<std::string, SLeaderboardEntry> Leaderboard;

    void LoadLeaderboard();
    void SaveLeaderboard();
    void UpdateLeaderboard();
};
