#include "World.hpp"

#include "Actors/BoxCollision.hpp"
#include "Actors/ComplexCollision.hpp"
#include "EngineStatics.hpp"


void CWorld::BeginPlay() {}

void CWorld::Tick(float DeltaTime) {
    for (const auto& Actor : Actors) {
        Actor->Tick(DeltaTime);
    }
    SolveCollisions(DeltaTime);
}

AActor* CWorld::SpawnActor(const TSubclassOf<AActor>& ActorClass) {
    AActor* NewActor = ActorClass.CreateObject();
    if (!NewActor) {
        Log("World", ELogLevel::Error, "Failed to spawn actor: Invalid class");
        return nullptr;
    }
    Actors.push_back(std::unique_ptr<AActor>(NewActor));
    return NewActor;
}

void CWorld::DestroyActor(AActor* Actor) {
    for (auto it = Actors.begin(); it != Actors.end(); ++it) {
        if (it->get() == Actor) {
            Actors.erase(it);
            return;
        }
    }
}

void CWorld::SolveCollisions(float DeltaTime) {
    for (const auto& Actor : GetActors<APhysicsBody>()) {
        if (!Actor->SimulatePhysics) continue;
        SVector Adjustment;
        for (const auto& Other : GetActors<APhysicsBody>()) {
            if (Actor == Other) continue;

            SVector Penetration;
            if (CheckCollision(Actor, Other, Penetration)) {
                if (Other->SimulatePhysics) {
                    Penetration *= 0.5f;
                }

                SVector AdjustmentForThisHit = Penetration;
                Actor->OnHit({Other, Penetration, DeltaTime}, AdjustmentForThisHit);
                Adjustment += AdjustmentForThisHit;
            }
        }

        if (Adjustment.LengthSquared() > SMALL_NUMBER) {
            Actor->Transform.Position += Adjustment;

            // Project velocity onto the plane perpendicular to the adjustment vector
            const SVector Normal = Adjustment.Normalized();
            const double VelocityAlongNormal = Actor->Velocity.Dot(Normal);

            if (VelocityAlongNormal < 0) {
                Actor->Velocity -= Normal * VelocityAlongNormal;
            }
        }
    }
}

bool CWorld::AreBoundingBoxesOverlapping(
    const SVector& PosA, const SVector& ExtentA, const SVector& PosB, const SVector& ExtentB
) const {
    return (std::abs(PosA.X - PosB.X) <= (ExtentA.X + ExtentB.X)) &&
           (std::abs(PosA.Y - PosB.Y) <= (ExtentA.Y + ExtentB.Y)) &&
           (std::abs(PosA.Z - PosB.Z) <= (ExtentA.Z + ExtentB.Z));
}

bool CWorld::CheckCollision(const APhysicsBody* A, const APhysicsBody* B, SVector& OutPenetration) const {
    const ABoxCollision* BoxA = Cast<ABoxCollision>(A);
    const ABoxCollision* BoxB = Cast<ABoxCollision>(B);
    const AComplexCollision* ComplexA = Cast<AComplexCollision>(A);
    const AComplexCollision* ComplexB = Cast<AComplexCollision>(B);

    if (BoxA && BoxB) {
        return CheckCollisionBoxBox(BoxA, BoxB, OutPenetration);
    }
    if (BoxA && ComplexB) {
        return CheckCollisionBoxComplex(BoxA, ComplexB, OutPenetration);
    }
    if (ComplexA && BoxB) {
        if (CheckCollisionBoxComplex(BoxB, ComplexA, OutPenetration)) {
            OutPenetration = -OutPenetration;
            return true;
        }
    }

    return false;
}

bool CWorld::CheckCollisionBoxBox(const ABoxCollision* A, const ABoxCollision* B, SVector& OutPenetration) const {
    OutPenetration = SVector(0);
    const SVector ToOther = (B->Transform.Position - A->Transform.Position).Abs();
    const SVector CombinedExtent = A->GetBoxExtent() + B->GetBoxExtent();
    if (ToOther.X < CombinedExtent.X && ToOther.Y < CombinedExtent.Y && ToOther.Z < CombinedExtent.Z) {
        SVector Penetration = CombinedExtent - ToOther;
        if (Penetration.X < Penetration.Y && Penetration.X < Penetration.Z) {
            OutPenetration.X = (A->Transform.Position.X < B->Transform.Position.X) ? -Penetration.X : Penetration.X;
        } else if (Penetration.Y < Penetration.Z) {
            OutPenetration.Y = (A->Transform.Position.Y < B->Transform.Position.Y) ? -Penetration.Y : Penetration.Y;
        } else {
            OutPenetration.Z = (A->Transform.Position.Z < B->Transform.Position.Z) ? -Penetration.Z : Penetration.Z;
        }
        return true;
    }
    return false;
}

bool CWorld::CheckCollisionBoxComplex(
    const ABoxCollision* Box, const AComplexCollision* Complex, SVector& OutPenetration
) const {
    const std::vector<SVector>& Vertices = Complex->Vertices;
    const std::vector<uint32>& Indices = Complex->Indices;

    const STransform& BoxTransform = Box->GetWorldTransform();
    const SVector BoxSemiextent = Box->GetBoxExtent() * BoxTransform.Scale;
    const SVector BoxCenter = BoxTransform.Position;

    bool bHasCollision = false;
    OutPenetration = SVector(0);

    for (int32 i = 0; i < Indices.size(); i += 3) {
        const STransform& ComplexTransform = Complex->GetWorldTransform();
        const SVector ComplexScale = ComplexTransform.Scale;
        const SVector ComplexPos = ComplexTransform.Position;

        SVector V0 = Vertices[Indices[i]];
        SVector V1 = Vertices[Indices[i + 1]];
        SVector V2 = Vertices[Indices[i + 2]];

        V0 = (V0 * ComplexScale).Rotated(ComplexTransform.Rotation) + ComplexPos;
        V1 = (V1 * ComplexScale).Rotated(ComplexTransform.Rotation) + ComplexPos;
        V2 = (V2 * ComplexScale).Rotated(ComplexTransform.Rotation) + ComplexPos;

        const SVector TriMin =
            SVector(std::min({V0.X, V1.X, V2.X}), std::min({V0.Y, V1.Y, V2.Y}), std::min({V0.Z, V1.Z, V2.Z}));
        const SVector TriMax =
            SVector(std::max({V0.X, V1.X, V2.X}), std::max({V0.Y, V1.Y, V2.Y}), std::max({V0.Z, V1.Z, V2.Z}));

        if (!AreBoundingBoxesOverlapping(TriMin, TriMax - TriMin, BoxCenter - BoxSemiextent, BoxSemiextent * 2.0)) {
            continue;
        }

        // SAT Axes
        const SVector E0 = V1 - V0;
        const SVector E1 = V2 - V1;
        const SVector E2 = V0 - V2;
        const SVector TriNormal = E0.Cross(E1);

        SVector Axes[13] = {SVector(1, 0, 0),           SVector(0, 1, 0),
                            SVector(0, 0, 1),           TriNormal,
                            SVector(1, 0, 0).Cross(E0), SVector(1, 0, 0).Cross(E1),
                            SVector(1, 0, 0).Cross(E2), SVector(0, 1, 0).Cross(E0),
                            SVector(0, 1, 0).Cross(E1), SVector(0, 1, 0).Cross(E2),
                            SVector(0, 0, 1).Cross(E0), SVector(0, 0, 1).Cross(E1),
                            SVector(0, 0, 1).Cross(E2)};

        double MinPenetration = DOUBLE_INF;
        SVector BestAxis = SVector(0);
        bool bIntersecting = true;

        for (const SVector& Axis : Axes) {
            if (Axis.LengthSquared() < SMALL_NUMBER) continue;
            const SVector NormAxis = Axis.Normalized(); // Important to normalize for correct depth

            // Project Box
            const double BoxRadius = BoxSemiextent.X * std::abs(NormAxis.X) + BoxSemiextent.Y * std::abs(NormAxis.Y) +
                                     BoxSemiextent.Z * std::abs(NormAxis.Z);

            const double BoxProj = BoxCenter.Dot(NormAxis);
            const double BoxMin = BoxProj - BoxRadius;
            const double BoxMax = BoxProj + BoxRadius;

            // Project Triangle
            double TriMinProj = DOUBLE_INF;
            double TriMaxProj = -DOUBLE_INF;
            for (const SVector& V : {V0, V1, V2}) {
                const double P = V.Dot(NormAxis);
                TriMinProj = std::min(TriMinProj, P);
                TriMaxProj = std::max(TriMaxProj, P);
            }

            // Test Separation
            if (BoxMax < TriMinProj || TriMaxProj < BoxMin) {
                bIntersecting = false;
                break;
            }

            // Calculate overlap (Minimum Translation Distance)
            const double Overlap = std::min(BoxMax - TriMinProj, TriMaxProj - BoxMin);
            if (Overlap < MinPenetration) {
                MinPenetration = Overlap;
                BestAxis = NormAxis;
                // Ensure axis points from Tri to Box
                if (BoxProj < (TriMinProj + TriMaxProj) * 0.5) {
                    BestAxis = -BestAxis;
                }
            }
        }

        if (bIntersecting) {
            SVector TriangleMTV = BestAxis * MinPenetration;

            // Push away from triangle center
            if (TriangleMTV.Dot(BoxCenter - V0) < 0) {
                TriangleMTV = -TriangleMTV;
            }

            if (TriangleMTV.LengthSquared() > OutPenetration.LengthSquared()) {
                OutPenetration = TriangleMTV;
            }

            bHasCollision = true;
        }
    }

    return bHasCollision;
}
