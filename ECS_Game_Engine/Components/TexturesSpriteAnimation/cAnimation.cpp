#include "cAnimation.h"
Animation::Animation(std::string_view animationName, Texture* sourceImage):
	m_name(animationName) , m_sourceImage(sourceImage){}

Animation::~Animation() {}

void Animation::play(){}
void Animation::pause(){}
const std::string& Animation::getName() const{
	return m_name;
}