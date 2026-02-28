#include "World.hpp"

#include "Actors/BoxCollision.hpp"
#include "Actors/ComplexCollision.hpp"
#include "EngineStatics.hpp"


void CWorld::BeginPlay() {}

void CWorld::Tick(float DeltaTime) {
    for (const auto& Actor : Actors) {
        if (!Actor->IsPendingDestroy) {
            Actor->Tick(DeltaTime);
        }
    }
    for (const auto& Actor : GetActors<APhysicsBody>()) {
        if (Actor->SimulatePhysics) {
            Actor->Transform.Position += Actor->Velocity * DeltaTime;
        }
    }
    SolveCollisions(DeltaTime);

    for (auto it = Actors.begin(); it != Actors.end();) {
        if ((*it)->IsPendingDestroy) {
            it = Actors.erase(it);
        } else {
            ++it;
        }
    }
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
    if (!Actor || Actor->IsPendingDestroy) return;

    Actor->IsPendingDestroy = true;

    for (const auto& A : Actors) {
        if (A->Parent == Actor) {
            DestroyActor(A.get());
        }
    }
}

bool CWorld::LineTrace(const SVector& Start, const SVector& End, SHitResult& OutHit) const {
    SVector Diff = End - Start;
    float MaxDist = Diff.Length();
    if (MaxDist < SMALL_NUMBER) return false;

    SVector Dir = Diff / MaxDist;

    bool HasHit = false;
    float ClosestDist = MaxDist;
    SHitResult ClosestHit;

    for (const auto& Actor : GetActors<APhysicsBody>()) {
        const ABoxCollision* Box = Cast<ABoxCollision>(Actor);
        const AComplexCollision* Complex = Cast<AComplexCollision>(Actor);

        float HitDist = 0.0f;
        SVector HitNormal = SVector(0);
        bool HitActor = false;

        if (Box) {
            const STransform& Transform = Box->GetWorldTransform();
            SVector Extents = Box->GetBoxExtent() * Transform.Scale;
            SVector AX = SVector(1, 0, 0).Rotated(Transform.Rotation);
            SVector AY = SVector(0, 1, 0).Rotated(Transform.Rotation);
            SVector AZ = SVector(0, 0, 1).Rotated(Transform.Rotation);

            if (CheckRayOBB(Start, Dir, MaxDist, Transform.Position, Extents, AX, AY, AZ, HitDist, HitNormal)) {
                HitActor = true;
            }
        } else if (Complex) {
            const std::vector<SVector>& Vertices = Complex->Vertices;
            const std::vector<uint32>& Indices = Complex->Indices;
            const STransform& Transform = Complex->GetWorldTransform();

            float MinComplexDist = MaxDist;
            SVector BestComplexNormal = SVector(0);
            bool HitAnyTri = false;

            for (size_t i = 0; i < Indices.size(); i += 3) {
                SVector V0 = Vertices[Indices[i]];
                SVector V1 = Vertices[Indices[i + 1]];
                SVector V2 = Vertices[Indices[i + 2]];

                V0 = (V0 * Transform.Scale).Rotated(Transform.Rotation) + Transform.Position;
                V1 = (V1 * Transform.Scale).Rotated(Transform.Rotation) + Transform.Position;
                V2 = (V2 * Transform.Scale).Rotated(Transform.Rotation) + Transform.Position;

                float TriDist = 0.0f;
                SVector TriNormal;

                if (CheckRayTriangle(Start, Dir, V0, V1, V2, TriDist, TriNormal)) {
                    if (TriDist < MinComplexDist) {
                        MinComplexDist = TriDist;
                        BestComplexNormal = TriNormal;
                        HitAnyTri = true;
                    }
                }
            }

            if (HitAnyTri) {
                HitActor = true;
                HitDist = MinComplexDist;
                HitNormal = BestComplexNormal;
            }
        }

        if (HitActor && HitDist < ClosestDist) {
            ClosestDist = HitDist;
            ClosestHit.OtherActor = Actor;
            ClosestHit.Distance = HitDist;
            ClosestHit.Normal = HitNormal;
            ClosestHit.Location = Start + Dir * HitDist;
            HasHit = true;
        }
    }

    if (HasHit) {
        OutHit = ClosestHit;
    }

    return HasHit;
}

void CWorld::SolveCollisions(float DeltaTime) {
    for (const auto& Actor : GetActors<APhysicsBody>()) {
        if (!Actor->SimulatePhysics || Actor->IsPendingDestroy) continue;
        SVector Adjustment;
        for (const auto& Other : GetActors<APhysicsBody>()) {
            if (Actor == Other || Other->IsPendingDestroy) continue;

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
    const STransform& TA = A->GetWorldTransform();
    const STransform& TB = B->GetWorldTransform();

    const SVector AX = SVector(1, 0, 0).Rotated(TA.Rotation);
    const SVector AY = SVector(0, 1, 0).Rotated(TA.Rotation);
    const SVector AZ = SVector(0, 0, 1).Rotated(TA.Rotation);

    const SVector BX = SVector(1, 0, 0).Rotated(TB.Rotation);
    const SVector BY = SVector(0, 1, 0).Rotated(TB.Rotation);
    const SVector BZ = SVector(0, 0, 1).Rotated(TB.Rotation);

    const SVector EA = A->GetBoxExtent() * TA.Scale;
    const SVector EB = B->GetBoxExtent() * TB.Scale;

    const SVector T = TB.Position - TA.Position;

    // SAT Axes
    SVector Axes[15] = {AX,           AY,           AZ,           BX,           BY,
                        BZ,           AX.Cross(BX), AX.Cross(BY), AX.Cross(BZ), AY.Cross(BX),
                        AY.Cross(BY), AY.Cross(BZ), AZ.Cross(BX), AZ.Cross(BY), AZ.Cross(BZ)};

    double MinOverlap = DOUBLE_INF;
    SVector BestAxis = SVector(0);

    for (const SVector& Axis : Axes) {
        if (Axis.LengthSquared() < SMALL_NUMBER) continue; // Skip near-zero axes (parallel edges)

        const SVector L = Axis.Normalized();

        // Project Box A radii onto axis L
        const double RA = EA.X * std::abs(AX.Dot(L)) + EA.Y * std::abs(AY.Dot(L)) + EA.Z * std::abs(AZ.Dot(L));

        // Project Box B radii onto axis L
        const double RB = EB.X * std::abs(BX.Dot(L)) + EB.Y * std::abs(BY.Dot(L)) + EB.Z * std::abs(BZ.Dot(L));

        // Project Distance T onto axis L (center-to-center projected distance)
        const double Dist = std::abs(T.Dot(L));

        const double Overlap = (RA + RB) - Dist;

        // If no overlap -> no collision
        if (Overlap <= 0) {
            OutPenetration = SVector(0);
            return false;
        }

        if (Overlap < MinOverlap) {
            MinOverlap = Overlap;
            BestAxis = L;

            // Ensure axis points from A to B (so pushing A away is negative)
            if (T.Dot(BestAxis) < 0) {
                BestAxis = -BestAxis;
            }
        }
    }

    // Since BestAxis points A -> B, the vector to push A OUT of B is -BestAxis
    OutPenetration = -BestAxis * MinOverlap;
    return true;
}

bool CWorld::CheckCollisionBoxComplex(
    const ABoxCollision* Box, const AComplexCollision* Complex, SVector& OutPenetration
) const {
    const std::vector<SVector>& Vertices = Complex->Vertices;
    const std::vector<uint32>& Indices = Complex->Indices;

    const STransform& BoxTransform = Box->GetWorldTransform();
    const SVector BoxSemiextent = Box->GetBoxExtent() * BoxTransform.Scale;
    const SVector BoxCenter = BoxTransform.Position;

    const SVector BX = SVector(1, 0, 0).Rotated(BoxTransform.Rotation);
    const SVector BY = SVector(0, 1, 0).Rotated(BoxTransform.Rotation);
    const SVector BZ = SVector(0, 0, 1).Rotated(BoxTransform.Rotation);

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

        const double BoxRadius = BoxSemiextent.Length();
        if (!AreBoundingBoxesOverlapping(
                TriMin, TriMax - TriMin, BoxCenter - SVector(BoxRadius), SVector(BoxRadius * 2.0)
            )) {
            continue;
        }

        const SVector E0 = V1 - V0;
        const SVector E1 = V2 - V1;
        const SVector E2 = V0 - V2;
        const SVector TriNormal = E0.Cross(E1);

        // SAT Axes
        SVector Axes[13] = {BX,           BY,           BZ,           TriNormal,    BX.Cross(E0),
                            BX.Cross(E1), BX.Cross(E2), BY.Cross(E0), BY.Cross(E1), BY.Cross(E2),
                            BZ.Cross(E0), BZ.Cross(E1), BZ.Cross(E2)};

        double MinPenetration = DOUBLE_INF;
        SVector BestAxis = SVector(0);
        bool bIntersecting = true;

        for (const SVector& Axis : Axes) {
            if (Axis.LengthSquared() < SMALL_NUMBER) continue;
            const SVector NormAxis = Axis.Normalized(); // Important to normalize for correct depth

            // Project Box
            const double BoxRadius = BoxSemiextent.X * std::abs(BX.Dot(NormAxis)) +
                                     BoxSemiextent.Y * std::abs(BY.Dot(NormAxis)) +
                                     BoxSemiextent.Z * std::abs(BZ.Dot(NormAxis));

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
