#include "PhysicsBody.hpp"

#include "EngineStatics.hpp"
#include "World.hpp"


void APhysicsBody::Tick(float DeltaTime) {
    if (SimulatePhysics) {
        Velocity += GetWorld()->Gravity * DeltaTime;
        CachedPosition = Transform.Position;
    }

    Super::Tick(DeltaTime);
}

void APhysicsBody::OnHit(const SHitResult& HitResult, SVector& OutAdjustment) {
    // Ground friction
    if (HitResult.Penetration.Normalized().Z > std::cos(ToRadians(45.0f))) {
        const SVector Normal = HitResult.Penetration.Normalized();
        const SVector ActualVelocity = (Transform.Position - CachedPosition) / HitResult.DeltaTime;
        const SVector TangentVelocity = ActualVelocity - Normal * ActualVelocity.Dot(Normal);

        if (TangentVelocity.LengthSquared() < Friction * Friction) {
            Velocity = SVector(0);

            // Revert horizontal movement (X/Y) to eliminate micro-sliding
            Transform.Position.X = CachedPosition.X;
            Transform.Position.Y = CachedPosition.Y;

            // Remove horizontal component of collision resolution
            const double OriginalDepth = HitResult.Penetration.Length();
            OutAdjustment = SVector();
            if (Normal.Z > SMALL_NUMBER) {
                OutAdjustment.Z = OriginalDepth / Normal.Z;
            }
        } else {
            const SVector FrictionForce = -TangentVelocity.Normalized() * Friction;
            Velocity += FrictionForce * HitResult.DeltaTime;
        }
    }
}
