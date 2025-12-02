#pragma once

#include "Actor.hpp"

#include <memory>
#include <vector>


class CWorld {
public:
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
