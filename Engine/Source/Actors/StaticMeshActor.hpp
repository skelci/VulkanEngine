#pragma once

#include "Actor.hpp"
#include "Assets/StaticMesh.hpp"
#include "Assets/Texture.hpp"

#include <memory>


class AStaticMeshActor : public AActor {
public:
    std::shared_ptr<CStaticMesh> StaticMesh;
    std::shared_ptr<CTexture> Texture;
};
