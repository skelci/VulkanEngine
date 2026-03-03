#include "GameMap.hpp"

#include "Actors/Boat.hpp"
#include "Actors/Character.hpp"
#include "Actors/Controller.hpp"
#include "Actors/Enemy.hpp"
#include "Actors/Teammate.hpp"
#include "Actors/Trash.hpp"
#include "Actors/WaterCollision.hpp"
#include "GameInstance.hpp"
#include "Maps/MenuMap.hpp"
#include "UI/DeathUI.hpp"
#include "UI/EscapeMenu.hpp"
#include "UI/LoadingScreen.hpp"
#include "UI/PlaytimeUI.hpp"
#include "UI/VictoryUI.hpp"

#include "Actors/BoxCollision.hpp"
#include "Actors/ComplexCollision.hpp"
#include "Actors/MeshActor.hpp"
#include "Assets/Material.hpp"
#include "Assets/Model.hpp"


void CGameMap::BeginPlay() {
    int32 Level = GEngine->GetGameInstance<CGameInstance>()->Level;

    GEngine->SimulationSpeed = 1.0f;
    GInputManager->SetInputMode(false);

    SInputMappingContext* GameMC = new SInputMappingContext();
    GameMC->AddMapping(
        EKeys::Escape,
        MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Pressed, this, &CGameMap::OnEscapePressed)
    );
    GameMC->AddMapping(
        EKeys::E,
        MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Pressed, this, &CGameMap::SwitchCharacterBoat)
    );
    GInputManager->AddMappingContext(GameMC);

    AMeshActor* Water = SpawnActor<AMeshActor>(AMeshActor::StaticClass());
    Water->Model = GetAsset<CModel>("Game/Meshes/HighResPlane.fbx");
    Water->Model->SetMaterial(GetAsset<CMaterial>("Game/Materials/Water.mat"));
    Water->Transform.Scale = SVector(1000, 1000, 1);

    AWaterCollision* WaterCollision = Water->SpawnChildActor<AWaterCollision>(AWaterCollision::StaticClass());

    AMeshActor* Sand = SpawnActor<AMeshActor>(AMeshActor::StaticClass());
    Sand->Model = GetAsset<CModel>("Game/Meshes/Island.fbx");
    Sand->Model->SetMaterial(GetAsset<CMaterial>("Game/Materials/Sand.mat"));
    Sand->Transform.Scale = SVector(100, 100, 100);
    Sand->Transform.Position = SVector(0.0f, 0.0f, -1.5f);

    AComplexCollision* SandCollision = Sand->SpawnChildActor<AComplexCollision>(AComplexCollision::StaticClass());
    SandCollision->SetCollisionMeshFromMesh(Sand->Model->Meshes[0]);

    Boat = SpawnActor<ABoat>(ABoat::StaticClass());
    Boat->Transform.Position = SVector(52, 52, 1);
    Boat->Transform.Rotation.Yaw = -45;

    Player = Cast<ACharacter>(SpawnAtRandomLocation(ACharacter::StaticClass()));

    Controller = SpawnActor<AController>(AController::StaticClass());
    Controller->Parent = Player;
    Controller->RotateTowardsMovement = true;
    Controller->RotationSpeed = 540;

    for (int32 i = 0; i < 5 * Level; i++) {
        const SVector RandomPos = SVector(Random::RandRange(-200.0, 200.0), Random::RandRange(-200.0, 200.0), 0);

        SHitResult HitResult;
        LineTrace(RandomPos + SVector(0, 0, 20), RandomPos + SVector(0, 0, -5), HitResult);
        if (!Cast<AWaterCollision>(HitResult.OtherActor)) {
            i--;
            continue;
        }

        ATrash* Trash = SpawnActor<ATrash>(ATrash::StaticClass());
        Trash->Transform.Position = RandomPos;
        Trash->Transform.Rotation.Yaw = Random::RandRange(0.0, 360.0);
    }

    for (int32 i = 0; i < 3 * Level; i++) {
        SpawnAtRandomLocation(AEnemy::StaticClass());
    }

    for (int32 i = 0; i < 2 * Level; i++) {
        SpawnAtRandomLocation(ATeammate::StaticClass());
    }

    GEngine->GetRenderer()->ClearUIWidgets();

    PlaytimeUI = GEngine->GetRenderer()->AddUIWidget<WPlaytimeUI>();
    PlaytimeUI->SetLevel(Level);
    PlaytimeUI->SetScore(0);
    PlaytimeUI->UpdateCounts(
        static_cast<int32>(GetActors<ATrash>().size()), static_cast<int32>(GetActors<AEnemy>().size())
    );
}

void CGameMap::AddScore(int32 Amount) {
    Score += Amount;
    if (PlaytimeUI) PlaytimeUI->SetScore(Score);
}

void CGameMap::OnEscapePressed() { ToggleEscapeMenu(); }

void CGameMap::ToggleEscapeMenu() {
    if (IsGameOver) return;

    if (EscapeMenuWidget) {
        GEngine->GetRenderer()->RemoveUIWidget(EscapeMenuWidget);
        EscapeMenuWidget = nullptr;
        GInputManager->SetInputMode(false);
        GEngine->SimulationSpeed = 1.0f;
        Controller->IsInputEnabled = true;
    } else {
        EscapeMenuWidget = GEngine->GetRenderer()->AddUIWidget<WEscapeMenu>();
        GInputManager->SetInputMode(true);
        GEngine->SimulationSpeed = 0.0f;
        Controller->IsInputEnabled = false;
    }
}

void CGameMap::SwitchCharacterBoat() {
    if (IsInBoat) {
        const STransform& BoatTransform = Boat->GetWorldTransform();
        const SVector NewPlayerPos = BoatTransform.Position - BoatTransform.Position.NormalizedXY() * 3;
        SHitResult HitResult;
        if (LineTrace(NewPlayerPos + SVector(0, 0, 1), NewPlayerPos + SVector(0, 0, -5), HitResult)) {
            if (!Cast<AComplexCollision>(HitResult.OtherActor)) {
                Log("Game", ELogLevel::Verbose, "Couldn't find a safe spot to exit the boat!");
                return;
            }
        }

        Player->Parent = nullptr;
        Player->Transform.Position = NewPlayerPos + SVector(0, 0, 1);
        Player->SimulatePhysics = true;
        Controller->Parent = Player;
        Controller->DisableSteering = false;
        Controller->RotationSpeed = 540;
        Controller->MovementSpeed = 6.0f;
    } else {
        if ((Player->GetWorldTransform().Position - Boat->GetWorldTransform().Position).Length() > 5) {
            Log("Game", ELogLevel::Verbose, "Too far from boat to enter!");
            return;
        }
        Player->Parent = Boat;
        Player->Transform.Position = SVector(0, 0, 0.8f);
        Player->Transform.Rotation = SRotator(0);
        Player->SimulatePhysics = false;
        Controller->Parent = Boat;
        Controller->DisableSteering = true;
        Controller->RotationSpeed = 45;
        Controller->MovementSpeed = 10.0f;
    }
    IsInBoat = !IsInBoat;
}

void CGameMap::OnEntitiesChanged() {
    if (IsGameOver) return;

    int32 TrashLeft = static_cast<int32>(GetActors<ATrash>().size());
    int32 EnemiesLeft = static_cast<int32>(GetActors<AEnemy>().size());
    PlaytimeUI->UpdateCounts(TrashLeft, EnemiesLeft);

    if (TrashLeft > 0 || EnemiesLeft > 0) return;

    // Level complete!
    IsGameOver = true;
    GInputManager->SetInputMode(true);
    GEngine->SimulationSpeed = 0.0f;
    Controller->IsInputEnabled = false;
    PlaytimeUI = nullptr;
    EscapeMenuWidget = nullptr;
    GEngine->GetRenderer()->ClearUIWidgets();
    GEngine->GetRenderer()->AddUIWidget<WVictoryUI>();
}

void CGameMap::OnPlayerDied() {
    if (IsGameOver) return;

    IsGameOver = true;
    GInputManager->SetInputMode(true);
    GEngine->SimulationSpeed = 0.0f;
    Controller->IsInputEnabled = false;
    PlaytimeUI = nullptr;
    EscapeMenuWidget = nullptr;
    GEngine->GetRenderer()->ClearUIWidgets();
    GEngine->GetRenderer()->AddUIWidget<WDeathUI>();
}

ABoxCollision* CGameMap::SpawnAtRandomLocation(const TSubclassOf<ABoxCollision>& ActorClass) {
    const SVector RandomPos = SVector(Random::RandRange(-100.0, 100.0), Random::RandRange(-100.0, 100.0), 0);

    SHitResult HitResult;
    LineTrace(RandomPos + SVector(0, 0, 20), RandomPos + SVector(0, 0, -5), HitResult);
    if (!Cast<AComplexCollision>(HitResult.OtherActor)) {
        return SpawnAtRandomLocation(ActorClass);
    }

    ABoxCollision* NewActor = SpawnActor<ABoxCollision>(ActorClass);
    NewActor->Transform.Position = HitResult.Location + SVector(0, 0, NewActor->GetBoxExtent().Z);
    NewActor->Transform.Rotation.Yaw = Random::RandRange(0.0, 360.0);
    return NewActor;
}
