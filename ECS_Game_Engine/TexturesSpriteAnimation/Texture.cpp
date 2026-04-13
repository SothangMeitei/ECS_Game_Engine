#include "Texture.h"

Texture::Texture(std::string& path, std::string& ID, SDL_Renderer*& renderer) :m_path(path), m_ID(ID) {
	m_surfaceFromCPU = IMG_Load(m_path.c_str());

	if (!m_surfaceFromCPU) {
		std::cerr << "Could not initialize the surface of the texture with the path: " << m_path;
		return;
	}

	m_TextureFromGPU = SDL_CreateTextureFromSurface(renderer, m_surfaceFromCPU);
}

Texture::~Texture() {
	if (m_TextureFromGPU != nullptr) {
		SDL_DestroyTexture(m_TextureFromGPU);
	}
	if (m_surfaceFromCPU != nullptr) {
		SDL_DestroySurface(m_surfaceFromCPU);
	}
}