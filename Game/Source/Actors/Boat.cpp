#include "Boat.hpp"

#include "Actors/ComplexCollision.hpp"
#include "Actors/MeshActor.hpp"
#include "Actors/Trash.hpp"
#include "Maps/GameMap.hpp"


ABoat::ABoat() {
    SimulatePhysics = true;
    Friction = 0.1f;
    SetBoxExtent(SVector(2.0f, 0.75f, 0.19f));
    SetVisibility(true);

    AMeshActor* Mesh = SpawnChildActor<AMeshActor>(AMeshActor::StaticClass());
    Mesh->Model = GetAsset<CModel>("Game/Meshes/Boat/gemiLow.fbx");
    Mesh->Transform.Rotation.Yaw = 90.0f;
    Mesh->Transform.Position.Z = 0.6f;
}

void ABoat::OnHit(const SHitResult& HitResult, SVector& OutAdjustment) {
    Super::OnHit(HitResult, OutAdjustment);

    if (Cast<AComplexCollision>(HitResult.OtherActor)) {
        const SVector Move = Transform.Position - CachedPosition;
        OutAdjustment = -Move;
        Velocity = SVector(0);
    }

    if (Cast<ATrash>(HitResult.OtherActor)) {
        CGameMap* GameMap = Cast<CGameMap>(GetWorld());
        GameMap->AddScore();
        GameMap->DestroyActor(HitResult.OtherActor);
    }
}
