#include "VikingRoom.hpp"

#include "Assets/Model.hpp"


AVikingRoom::AVikingRoom() {
    Model = GetAsset<CModel>("Game/viking_room/viking_room.obj");
    Model->SetTexture(GetAsset<CTexture>("Game/viking_room/viking_room.png"));
}
