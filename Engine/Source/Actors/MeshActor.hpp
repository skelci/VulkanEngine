#pragma once

#include "Actor.hpp"
#include "Assets/Model.hpp"

#include <memory>


class AMeshActor : public AActor {
    CLASS_BODY(AMeshActor, AActor)

public:
    std::shared_ptr<CModel> Model;
};
