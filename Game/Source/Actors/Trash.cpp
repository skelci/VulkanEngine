#include "Trash.hpp"

#include "Actors/MeshActor.hpp"


ATrash::ATrash() {
    AMeshActor* TrashMesh = SpawnChildActor<AMeshActor>(AMeshActor::StaticClass());
    TrashMesh->Model = GetAsset<CModel>("Game/Meshes/Trash.glb");
    TrashMesh->Transform.Scale = SVector(0.01f);
}
