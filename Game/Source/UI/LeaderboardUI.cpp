#include "LeaderboardUI.hpp"

#include "GameInstance.hpp"
#include "MainMenu.hpp"

#include "Widgets/Button.hpp"
#include "Widgets/HorizontalBox.hpp"
#include "Widgets/Image.hpp"
#include "Widgets/Text.hpp"
#include "Widgets/VerticalBox.hpp"


WLeaderboardUI::WLeaderboardUI() {
    // Dark background
    WImage* Bg = AddChild<WImage>();
    Bg->SetMaterialProperty("Color", SVector4(0.1f, 0.1f, 0.15f));
    GetSlotForChild(Bg)->Alignment = SVector4(0, 1, 0, 1);

    // Content panel centered
    WOverlay* Panel = AddChild<WOverlay>();
    GetSlotForChild(Panel)->Alignment = SVector4(0.5f);

    WImage* PanelBg = Panel->AddChild<WImage>();
    PanelBg->Size = SVector2(500, 500);
    PanelBg->SetMaterialProperty("Color", SVector4(0.15f, 0.15f, 0.2f));
    PanelBg->SetMaterialProperty("CornerRadius", SVector4(12));
    Panel->GetSlotForChild(PanelBg)->Alignment = SVector4(0, 1, 0, 1);

    WVerticalBox* VBox = Panel->AddChild<WVerticalBox>();
    Panel->GetSlotForChild(VBox)->Alignment = SVector4(0, 1, 0, 1);
    Panel->GetSlotForChild(VBox)->Padding = SVector4(20);

    // Title
    WText* Title = VBox->AddChild<WText>();
    Title->SetText("Leaderboard");
    Title->FontSize = 36.0f;
    Title->SetMaterialProperty("Color", SVector4(1.0f, 0.85f, 0.2f));
    VBox->GetSlotForChild(Title)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(Title)->Padding = SVector4(0, 0, 0, 15);

    // Leaderboard entries
    WHorizontalBox* Entries = VBox->AddChild<WHorizontalBox>();
    VBox->GetSlotForChild(Entries)->Alignment = SVector2(0.0f, 1.0f);
    VBox->GetSlotForChild(Entries)->Padding = SVector4(20, 0, 20, 0);

    // Username entries
    WVerticalBox* Names = Entries->AddChild<WVerticalBox>();
    Entries->GetSlotForChild(Names)->Alignment = SVector2(0.0f, 1.0f);
    Entries->GetSlotForChild(Names)->Padding = SVector4(5, 0, 5, 0);

    // Names header
    WText* NamesHeader = Names->AddChild<WText>();
    NamesHeader->SetText("Name");
    NamesHeader->FontSize = 22.0f;
    NamesHeader->SetMaterialProperty("Color", SVector4(0.7f, 0.7f, 0.7f));
    Names->GetSlotForChild(NamesHeader)->Padding = SVector4(0, 0, 0, 5);

    // Level entries
    WVerticalBox* Levels = Entries->AddChild<WVerticalBox>();
    Entries->GetSlotForChild(Levels)->Alignment = SVector2(0.0f, 1.0f);
    Entries->GetSlotForChild(Levels)->Padding = SVector4(5, 0, 5, 0);

    // Levels header
    WText* LevelsHeader = Levels->AddChild<WText>();
    LevelsHeader->SetText("Level");
    LevelsHeader->FontSize = 22.0f;
    LevelsHeader->SetMaterialProperty("Color", SVector4(0.7f, 0.7f, 0.7f));
    Levels->GetSlotForChild(LevelsHeader)->Padding = SVector4(100, 0, 100, 5);

    // Score entries
    WVerticalBox* Scores = Entries->AddChild<WVerticalBox>();
    Entries->GetSlotForChild(Scores)->Alignment = SVector2(0.0f, 1.0f);
    Entries->GetSlotForChild(Scores)->Padding = SVector4(5, 0, 5, 0);

    // Scores header
    WText* ScoresHeader = Scores->AddChild<WText>();
    ScoresHeader->SetText("Score");
    ScoresHeader->FontSize = 22.0f;
    ScoresHeader->SetMaterialProperty("Color", SVector4(0.7f, 0.7f, 0.7f));
    Scores->GetSlotForChild(ScoresHeader)->Padding = SVector4(0, 0, 0, 5);

    // Load data
    CGameInstance* GI = GEngine->GetGameInstance<CGameInstance>();
    GI->LoadLeaderboard();

    // Sort by TotalScore descending
    std::vector<std::pair<std::string, SLeaderboardEntry>> Sorted(GI->Leaderboard.begin(), GI->Leaderboard.end());
    std::sort(Sorted.begin(), Sorted.end(), [](const auto& A, const auto& B) {
        return A.second.TotalScore > B.second.TotalScore;
    });

    int32 Count = 0;
    for (const auto& [Name, Entry] : Sorted) {
        if (Count >= 10) break;
        Count++;

        std::string LevelStr = std::to_string(Entry.Level);
        std::string ScoreStr = std::to_string(Entry.TotalScore);

        Log("Temp", ELogLevel::Debug,
            "Adding leaderboard entry: " + Name + " - Level " + LevelStr + " - Score " + ScoreStr);
        WText* NameRow = Names->AddChild<WText>();
        NameRow->SetText(Name);
        NameRow->FontSize = 18.0f;
        NameRow->SetMaterialProperty("Color", SVector4(0.9f, 0.9f, 0.9f));
        Names->GetSlotForChild(NameRow)->Padding = SVector4(0, 2, 0, 2);

        WText* LevelRow = Levels->AddChild<WText>();
        LevelRow->SetText(LevelStr);
        LevelRow->FontSize = 18.0f;
        LevelRow->SetMaterialProperty("Color", SVector4(0.9f, 0.9f, 0.9f));
        Levels->GetSlotForChild(LevelRow)->Alignment = SVector2(0.5f, 0.5f);
        Levels->GetSlotForChild(LevelRow)->Padding = SVector4(0, 2, 0, 2);

        WText* ScoreRow = Scores->AddChild<WText>();
        ScoreRow->SetText(ScoreStr);
        ScoreRow->FontSize = 18.0f;
        ScoreRow->SetMaterialProperty("Color", SVector4(0.9f, 0.9f, 0.9f));
        Scores->GetSlotForChild(ScoreRow)->Alignment = SVector2(0.5f, 0.5f);
        Scores->GetSlotForChild(ScoreRow)->Padding = SVector4(0, 2, 0, 2);
    }

    if (Sorted.empty()) {
        WText* Empty = VBox->AddChild<WText>();
        Empty->SetText("No entries yet!");
        Empty->FontSize = 20.0f;
        Empty->SetMaterialProperty("Color", SVector4(0.6f, 0.6f, 0.6f));
        VBox->GetSlotForChild(Empty)->Alignment = SVector2(0.5f, 0.5f);
        VBox->GetSlotForChild(Empty)->Padding = SVector4(0, 20, 0, 0);
    }

    // Back button
    WButton* BackBtn = VBox->AddChild<WButton>();
    BackBtn->Size = SVector2(200, 45);
    BackBtn->SetText("Back");
    BackBtn->SetTextMaterialProperty("Color", SVector4(1));
    BackBtn->GetImageWidget()->SetMaterialProperty("Color", SVector4(0.3f, 0.3f, 0.5f));
    BackBtn->GetImageWidget()->SetMaterialProperty("CornerRadius", SVector4(8));
    VBox->GetSlotForChild(BackBtn)->Alignment = SVector2(0.5f, 0.5f);
    VBox->GetSlotForChild(BackBtn)->Padding = SVector4(0, 15, 0, 0);
    BackBtn->OnClickedDelegate.Bind([this]() {
        GEngine->GetRenderer()->ClearUIWidgets();
        GEngine->GetRenderer()->AddUIWidget<WMainMenu>();
    });
}
