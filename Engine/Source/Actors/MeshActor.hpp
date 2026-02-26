#pragma once

#include "Actor.hpp"
#include "Assets/Material.hpp"
#include "Assets/Model.hpp"

#include <memory>


class AMeshActor : public AActor {
    CLASS_BODY(AMeshActor, AActor)

public:
    AMeshActor();

    std::shared_ptr<CModel> Model;
    bool IsVisible = true;
};
