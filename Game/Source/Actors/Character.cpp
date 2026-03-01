#include "Character.hpp"

#include "Actors/MeshActor.hpp"
#include "Actors/WaterCollision.hpp"


ACharacter::ACharacter() {
    SimulatePhysics = true;
    Friction = 1;
    SetBoxExtent(SVector(0.25f, 0.25f, 0.6f));
    SetVisibility(true);

    Mesh = SpawnChildActor<AMeshActor>(AMeshActor::StaticClass());
    Mesh->Model = GetAsset<CModel>("Game/Character/burnice.fbx", true);
    Mesh->Model->SetMaterialProperty("LightColor", SVector3(1.5f, 1.5f, 1.5f));
    Mesh->Model->SetMaterialProperty("AmbientLight", SVector3(0.5f));
    Mesh->Transform.Rotation.Yaw = 90.0f;
    Mesh->Transform.Position.Z = -0.57f;
}

void ACharacter::OnHit(const SHitResult& HitResult, SVector& OutAdjustment) {
    Super::OnHit(HitResult, OutAdjustment);

    if (Cast<AWaterCollision>(HitResult.OtherActor)) {
        const SVector Move = Transform.Position - CachedPosition;
        OutAdjustment = -Move;
        Velocity = SVector(0);
    }
}
