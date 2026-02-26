#include "Actor.hpp"

#include "EngineStatics.hpp"


STransform AActor::GetWorldTransform() const {
    if (!Parent) {
        return Transform;
    }

    const STransform parentTransform = Parent->GetWorldTransform();
    STransform worldTransform;
    worldTransform.Scale = parentTransform.Scale * Transform.Scale;
    worldTransform.Rotation = parentTransform.Rotation + Transform.Rotation;
    worldTransform.Position =
        parentTransform.Position + Transform.Position.Rotated(parentTransform.Rotation) * parentTransform.Scale;
    return worldTransform;
}

AActor* AActor::SpawnChildActor(const TSubclassOf<AActor>& ActorClass) {
    AActor* Actor = GetWorld()->SpawnActor(ActorClass);
    Actor->Parent = this;
    return Actor;
}

CWorld* AActor::GetWorld() const { return GEngine->GetWorld(); }
