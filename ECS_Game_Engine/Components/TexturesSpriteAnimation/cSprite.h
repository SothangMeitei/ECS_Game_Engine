#pragma once
#include <string>
#include <SDL3/SDL.h>
//some things that are non moving that is static and stetionary in place
struct cSprite {
    std::string textureID; // e.g., "BrickWall"

    cSprite(const std::string& id) : textureID(id) {}
};