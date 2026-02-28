#pragma once

#include "CoreMinimal.hpp"
#include "Widgets/Overlay.hpp"


class WPlaytimeUI : public WOverlay {
    CLASS_BODY(WPlaytimeUI, WOverlay)

public:
    WPlaytimeUI();

    void SetScore(int32 Score);

protected:
    class WText* ScoreText;
};
