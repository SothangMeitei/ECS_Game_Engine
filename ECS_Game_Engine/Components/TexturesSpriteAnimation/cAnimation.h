#pragma once
#include <string>
#include <SDL3/SDL.h>

struct AnimState {
    int frameCount;
    float frameDuration;
    SDL_FRect startRect;
};

struct cAnimation {
    std::string                                 textureID;     
    std::unordered_map<std::string, AnimState>  states;

    std::string     currentAnimName;
    int             currentFrame = 0;      // Which frame are we on?
    float           timeElapsed = 0.0f;  // Internal timer

    void changeState(const std::string& newStateName) {
        if (currentAnimName != newStateName) {
            currentAnimName = newStateName;
            currentFrame = 0;
            timeElapsed = 0.0f;
        }
    }
    cAnimation(const std::string& textureID) : textureID(textureID) {}
};

