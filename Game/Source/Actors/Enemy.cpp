#include "Enemy.hpp"

#include "Actors/MeshActor.hpp"
#include "Actors/Trash.hpp"
#include "Actors/WaterCollision.hpp"
#include "Maps/GameMap.hpp"


AEnemy::AEnemy() {
    Mesh->Model->SetMaterialProperty("Color", SVector4(1, 0.5, 0.5));
    Mesh->Model->SetMaterialProperty("FadeDistance", 15.0f);
}

void AEnemy::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    CGameMap* GameMap = Cast<CGameMap>(GetWorld());
    const ACharacter* Player = GameMap->Player;

    const SVector ToPlayer = Player->GetWorldTransform().Position - GetWorldTransform().Position;

    if (ToPlayer.Length() < 10.0f) {
        const SVector DesiredVelocity = -ToPlayer.NormalizedXY() * 3.0f;
        Velocity.X = DesiredVelocity.X;
        Velocity.Y = DesiredVelocity.Y;
    } else {
        Velocity.X = 0;
        Velocity.Y = 0;
    }
}

void AEnemy::OnHit(const SHitResult& HitResult, SVector& OutAdjustment) {
    Super::OnHit(HitResult, OutAdjustment);

    const STransform& WorldTransform = GetWorldTransform();

    if (HasTrash && Cast<AWaterCollision>(HitResult.OtherActor)) {
        HasTrash = false;

        ATrash* Trash = GetWorld()->SpawnActor<ATrash>(ATrash::StaticClass());
        Trash->Transform.Position = WorldTransform.Position + WorldTransform.Position.NormalizedXY() * 2;
        Trash->Transform.Rotation.Yaw = Random::RandRange(0.0, 360.0);
    }

    CGameMap* GameMap = Cast<CGameMap>(GetWorld());
    if (HitResult.OtherActor != GameMap->Player) {
        return;
    }

    for (const auto& Actor : GetWorld()->GetActors<AEnemy>()) {
        if (Actor == this) continue;

        const SVector ToOther = Actor->GetWorldTransform().Position - WorldTransform.Position;
        const float Dist = ToOther.Length();
        if (Dist < 10.0f) {
            Log("Game", ELogLevel::Info, "Enemy killed you! Restarting level...");
            GEngine->OpenWorld(CGameMap::StaticClass());
            return;
        }
    }

    GameMap->AddScore();
    GameMap->DestroyActor(this);
};
