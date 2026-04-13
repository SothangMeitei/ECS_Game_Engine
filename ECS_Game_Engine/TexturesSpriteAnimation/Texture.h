#pragma once
#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

//sdl surface is the image that lives on the cpu or the main ram
//sdl texture lives on the vram on the gpu

//since all the things must go throught the cpu the image is to be initialized using the sdl surface and then 
//this surface is used to initialize the texture , once the texture lives on the gpu the surface is deleted

//the loading path is thus, hard drive -> cpu -> gpu
//when the 
class Texture
{
private:
	std::string m_path;
	std::string m_ID;
	SDL_Surface* m_surfaceFromCPU;
	SDL_Texture* m_TextureFromGPU;

public:
	Texture(std::string& path, std::string& ID, SDL_Renderer*& renderer);
	~Texture();

	std::string& getID() { return m_ID; }
};

