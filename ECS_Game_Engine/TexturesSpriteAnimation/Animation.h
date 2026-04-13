#pragma once
#include"Texture.h"
#include<string>

class Animation
{
private:
	std::string m_name;
	Texture* m_sourceImage;
	//std::shared_ptr<Texture> m_soruceImage;

public:
	Animation(std::string_view ,Texture*);
	~Animation();

	void play();
	void pause();
	const std::string& getName() const;
};

