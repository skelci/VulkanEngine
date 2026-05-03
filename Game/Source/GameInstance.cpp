#include "GameInstance.hpp"

#include <fstream>


void CGameInstance::LoadLeaderboard() {
    Leaderboard.clear();
    std::ifstream File("Saved/Leaderboard.bin", std::ios::binary);
    if (!File.is_open()) return;
    uint32 Count = 0;
    if (!File.read(reinterpret_cast<char*>(&Count), sizeof(Count))) return;
    for (uint32 i = 0; i < Count; ++i) {
        uint32 NameLen = 0;
        if (!File.read(reinterpret_cast<char*>(&NameLen), sizeof(NameLen))) break;
        std::string Name;
        if (NameLen > 0) {
            Name.resize(NameLen);
            File.read(&Name[0], NameLen);
        }
        int32 Lvl = 0, Score = 0;
        File.read(reinterpret_cast<char*>(&Lvl), sizeof(Lvl));
        File.read(reinterpret_cast<char*>(&Score), sizeof(Score));
        Leaderboard[Name] = {Lvl, Score};
        if (Name == Username) {
            Level = Lvl;
            TotalScore = Score;
        }
    }
}

void CGameInstance::SaveLeaderboard() {
    std::ofstream File("Saved/Leaderboard.bin", std::ios::binary);
    if (!File.is_open()) return;
    uint32 Count = static_cast<uint32>(Leaderboard.size());
    File.write(reinterpret_cast<const char*>(&Count), sizeof(Count));
    for (const auto& [Name, Entry] : Leaderboard) {
        uint32 NameLen = static_cast<uint32>(Name.size());
        File.write(reinterpret_cast<const char*>(&NameLen), sizeof(NameLen));
        if (NameLen > 0) File.write(Name.data(), NameLen);
        int32 Lvl = Entry.Level;
        int32 Score = Entry.TotalScore;
        File.write(reinterpret_cast<const char*>(&Lvl), sizeof(Lvl));
        File.write(reinterpret_cast<const char*>(&Score), sizeof(Score));
    }
}

void CGameInstance::UpdateLeaderboard() {
    auto& Entry = Leaderboard[Username];
    if (Level > Entry.Level) Entry.Level = Level;
    if (TotalScore > Entry.TotalScore) Entry.TotalScore = TotalScore;
    SaveLeaderboard();
}
