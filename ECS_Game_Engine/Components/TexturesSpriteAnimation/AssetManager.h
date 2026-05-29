#pragma once
#include<SDL3/SDL.h>
#include<SDL3_image/SDL_image.h>
#include<string>
#include<unordered_map>

class AssetManager {
    std::unordered_map<std::string, SDL_Texture*> m_textures;
public:
    ~AssetManager() {
        for (auto& pair : m_textures) {
            SDL_DestroyTexture(pair.second);
        }
        m_textures.clear();
    }
    void addTexture(const std::string& id, const std::string& path, SDL_Renderer* renderer) {
        if (m_textures.find(id) != m_textures.end()) {
            std::cout << "Warning: Texture " << id << " already exist\n";
            return;
        }

        SDL_Surface* surface = IMG_Load(path.c_str());  //load form the drive to the cpu
        if (!surface) {
            std::cout << "Failed to load the image at " << path << " | SDL error: " << SDL_GetError() << "\n";
            return;
        }
        
        Uint32 magicPink = SDL_MapSurfaceRGB(surface, 255, 0, 255);
        SDL_SetSurfaceColorKey(surface, true, magicPink);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);

        if (!texture) {
            std::cerr << "Failed to create texture form " << path << " | SDL_Error: " << SDL_GetError() << '\n';
            return;
        }

        m_textures[id] = texture;
    }
    SDL_Texture* getTexture(const std::string& id) {
        return m_textures[id];
    }
};