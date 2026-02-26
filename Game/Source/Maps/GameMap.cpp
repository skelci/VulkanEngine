#include "GameMap.hpp"

#include "Actors/Character.hpp"

#include "Actors/BoxCollision.hpp"
#include "Actors/ComplexCollision.hpp"
#include "Actors/MeshActor.hpp"
#include "Assets/Material.hpp"
#include "Assets/Model.hpp"


void CGameMap::BeginPlay() {
    GInputManager->SetInputMode(false);

    SInputMappingContext* GameMC = new SInputMappingContext();
    GameMC->AddMapping(
        EKeys::Escape,
        MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Pressed, this, &CGameMap::OnEscapePressed)
    );
    GInputManager->AddMappingContext(GameMC);

    AMeshActor* Water = SpawnActor<AMeshActor>(AMeshActor::StaticClass());
    Water->Model = GetAsset<CModel>("Game/Meshes/HighResPlane.fbx");
    Water->Model->SetMaterial(GetAsset<CMaterial>("Game/Materials/Water.mat"));
    Water->Transform.Scale = SVector(1000, 1000, 1);

    AMeshActor* Sand = SpawnActor<AMeshActor>(AMeshActor::StaticClass());
    Sand->Model = GetAsset<CModel>("Game/Meshes/Island.fbx");
    Sand->Model->SetMaterial(GetAsset<CMaterial>("Game/Materials/Sand.mat"));
    Sand->Transform.Scale = SVector(100, 100, 100);
    Sand->Transform.Position = SVector(0.0f, 0.0f, -1.5f);

    AComplexCollision* SandCollision = Sand->SpawnChildActor<AComplexCollision>(AComplexCollision::StaticClass());
    SandCollision->SetCollisionMeshFromMesh(Sand->Model->Meshes[0]);
    SandCollision->SetVisibility(true);

    ACharacter* Player = SpawnActor<ACharacter>(ACharacter::StaticClass());
    Player->Transform.Position = SVector(0, 0, 20);
}

void CGameMap::OnEscapePressed() { GEngine->Stop(); }
