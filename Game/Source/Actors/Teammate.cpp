#include "Teammate.hpp"

#include "Actors/MeshActor.hpp"
#include "Maps/GameMap.hpp"


ATeammate::ATeammate() {
    SimulatePhysics = true;
    Mesh->Model->SetMaterialProperty("Color", SVector4(0.5, 1, 0.5));
}

void ATeammate::OnHit(const SHitResult& HitResult, SVector& OutAdjustment) {
    Super::OnHit(HitResult, OutAdjustment);

    CGameMap* GameMap = Cast<CGameMap>(GetWorld());
    if (HitResult.OtherActor != GameMap->Player) {
        return;
    }

    GameMap->AddScore(-2);
    GameMap->DestroyActor(this);
}
