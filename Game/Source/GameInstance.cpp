#include "GameInstance.hpp"

#include <fstream>
#include <sstream>


void CGameInstance::LoadLeaderboard() {
    Leaderboard.clear();
    std::ifstream File("Saved/Leaderboard.txt");
    if (!File.is_open()) return;
    std::string Line;
    while (std::getline(File, Line)) {
        std::istringstream SS(Line);
        std::string Name;
        int32 Lvl = 0, Score = 0;
        if (SS >> Name >> Lvl >> Score) {
            Leaderboard[Name] = {Lvl, Score};
        }
        if (Name == Username) {
            Level = Lvl;
            TotalScore = Score;
        }
    }
}

void CGameInstance::SaveLeaderboard() {
    std::ofstream File("Saved/Leaderboard.txt");
    if (!File.is_open()) return;
    for (const auto& [Name, Entry] : Leaderboard) {
        File << Name << " " << Entry.Level << " " << Entry.TotalScore << "\n";
    }
}

void CGameInstance::UpdateLeaderboard() {
    auto& Entry = Leaderboard[Username];
    if (Level > Entry.Level) Entry.Level = Level;
    if (TotalScore > Entry.TotalScore) Entry.TotalScore = TotalScore;
    SaveLeaderboard();
}
