#include "Trash.hpp"

#include "Actors/MeshActor.hpp"


ATrash::ATrash() {
    SimulatePhysics = true;
    Friction = 0;
    SetBoxExtent(SVector(0.5f, 0.5f, 0.4f));

    AMeshActor* TrashMesh = SpawnChildActor<AMeshActor>(AMeshActor::StaticClass());
    TrashMesh->Model = GetAsset<CModel>("Game/Meshes/Trash.glb");
    TrashMesh->Transform.Scale = SVector(0.01f);
    TrashMesh->Transform.Position = SVector(0, 0, -0.5f);
}

void ATrash::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (Random::Rand() * DeltaTime < 0.2f) {
        Velocity.X += Random::RandRange(-0.5, 0.5);
        Velocity.Y += Random::RandRange(-0.5, 0.5);
    }

    if (Velocity.Length() > 5.0f) {
        Velocity /= 2;
    }

    const STransform& WorldTransform = GetWorldTransform();
    const double Dist = WorldTransform.Position.Length();
    if (Dist > 200.0f) {
        Velocity += -WorldTransform.Position.NormalizedXY() * 5.0f;
    }
    if (Dist < 100.0f) {
        Velocity += WorldTransform.Position.NormalizedXY() * 2.5f;
    }
}
