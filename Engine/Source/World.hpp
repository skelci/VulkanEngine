#pragma once

#include "Actors/Actor.hpp"
#include "Object.hpp"

#include <memory>
#include <vector>


class CWorld : public CObject {
    CLASS_BODY(CWorld, CObject)

protected:
    friend class CEngine;
    virtual void BeginPlay();

public:
    virtual void Tick(float DeltaTime);

    template <typename T>
    T* SpawnActor() {
        static_assert(std::is_base_of_v<AActor, T>, "T must be derived from AActor");

        T* NewActor = new T();
        Actors.push_back(std::unique_ptr<AActor>(NewActor));
        return NewActor;
    }

    const std::vector<std::unique_ptr<AActor>>& GetActors() const { return Actors; }

private:
    std::vector<std::unique_ptr<AActor>> Actors;
};
