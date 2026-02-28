#include "PlaytimeUI.hpp"

#include "Widgets/Text.hpp"


WPlaytimeUI::WPlaytimeUI() {
    ScoreText = AddChild<WText>();
    ScoreText->FontSize = 32.0f;
    SOverlaySlot* Slot = GetSlotForChild(ScoreText);
    Slot->Padding = SVector4(10);

    SetScore(0);
}

void WPlaytimeUI::SetScore(int32 Score) { ScoreText->SetText("Score: " + std::to_string(Score)); }
