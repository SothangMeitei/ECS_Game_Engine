#pragma once
#include<SDL3/SDL.h>
#include<string>
#include<unordered_map>

class AssetManager {
    std::unordered_map<std::string, SDL_Texture*> m_textures;
public:
    void addTexture(const std::string& id, const std::string& path, SDL_Renderer* renderer) {
        // Load the surface, create texture, DESTROY surface, store in m_textures
    }
    SDL_Texture* getTexture(const std::string& id) {
        return m_textures[id];
    }
};