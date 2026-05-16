#pragma once
#include"cTexture.h"
#include<string>
#include<SDL3/SDL.h>

class Animation
{
private:
	std::string m_name;
	Texture*	m_sourceImage;

	SDL_FRect	m_sourceRectangle;
	SDL_FRect	m_initialSourceRectangle;
	SDL_FRect	m_destinationRectagle;

	int			m_animationSpeed;
	int			m_numberOfFrames;
public:
	Animation(std::string_view ,Texture*);
	~Animation();

	void play();
	void pause();
	const std::string& getName() const;
};

