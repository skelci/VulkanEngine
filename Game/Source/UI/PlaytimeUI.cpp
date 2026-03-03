#include "PlaytimeUI.hpp"

#include "Widgets/Image.hpp"
#include "Widgets/Text.hpp"
#include "Widgets/VerticalBox.hpp"


WPlaytimeUI::WPlaytimeUI() {
    // Semi-transparent panel in top-left
    WOverlay* Panel = AddChild<WOverlay>();
    SOverlaySlot* PanelSlot = GetSlotForChild(Panel);
    PanelSlot->Alignment = SVector4(0.0f, 0.0f, 0.0f, 0.0f);

    WImage* PanelBg = Panel->AddChild<WImage>();
    PanelBg->Size = SVector2(220, 140);
    PanelBg->SetMaterialProperty("Color", SVector4(0.0f, 0.0f, 0.0f, 0.5f));
    PanelBg->SetMaterialProperty("CornerRadius", SVector4(0, 10, 0, 0));
    Panel->GetSlotForChild(PanelBg)->Alignment = SVector4(0, 1, 0, 1);

    WVerticalBox* VBox = Panel->AddChild<WVerticalBox>();
    Panel->GetSlotForChild(VBox)->Alignment = SVector4(0, 1, 0, 1);
    Panel->GetSlotForChild(VBox)->Padding = SVector4(12, 8, 12, 8);

    LevelText = VBox->AddChild<WText>();
    LevelText->FontSize = 22.0f;
    LevelText->SetMaterialProperty("Color", SVector4(0.2f, 0.8f, 1.0f));
    VBox->GetSlotForChild(LevelText)->Padding = SVector4(0, 2, 0, 2);

    TrashText = VBox->AddChild<WText>();
    TrashText->FontSize = 20.0f;
    TrashText->SetMaterialProperty("Color", SVector4(0.9f, 0.9f, 0.5f));
    VBox->GetSlotForChild(TrashText)->Padding = SVector4(0, 2, 0, 2);

    EnemyText = VBox->AddChild<WText>();
    EnemyText->FontSize = 20.0f;
    EnemyText->SetMaterialProperty("Color", SVector4(1.0f, 0.5f, 0.5f));
    VBox->GetSlotForChild(EnemyText)->Padding = SVector4(0, 2, 0, 2);

    ScoreText = VBox->AddChild<WText>();
    ScoreText->FontSize = 22.0f;
    ScoreText->SetMaterialProperty("Color", SVector4(1.0f, 0.85f, 0.2f));
    VBox->GetSlotForChild(ScoreText)->Padding = SVector4(0, 2, 0, 2);

    SetLevel(1);
    SetScore(0);
    UpdateCounts(0, 0);
}

void WPlaytimeUI::SetScore(int32 Score) { ScoreText->SetText("Score: " + std::to_string(Score)); }

void WPlaytimeUI::SetLevel(int32 Level) { LevelText->SetText("Level: " + std::to_string(Level)); }

void WPlaytimeUI::UpdateCounts(int32 TrashLeft, int32 EnemiesLeft) {
    TrashText->SetText("Trash: " + std::to_string(TrashLeft));
    EnemyText->SetText("Enemies: " + std::to_string(EnemiesLeft));
}
