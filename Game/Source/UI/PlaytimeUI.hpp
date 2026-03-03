#pragma once

#include "CoreMinimal.hpp"
#include "Widgets/Overlay.hpp"


class WPlaytimeUI : public WOverlay {
    CLASS_BODY(WPlaytimeUI, WOverlay)

public:
    WPlaytimeUI();

    void SetScore(int32 Score);
    void SetLevel(int32 Level);
    void UpdateCounts(int32 TrashLeft, int32 EnemiesLeft);

protected:
    class WText* LevelText;
    class WText* TrashText;
    class WText* EnemyText;
    class WText* ScoreText;
};
