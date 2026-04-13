#pragma once
#include"ECS/EntityManager.h"
#include"TexturesSpriteAnimation/Animation.h"
#include"TexturesSpriteAnimation/Texture.h"
#include<unordered_map>
#include"GameEngine.h"

class AbstractScene
{
protected:
	 EntityManager	m_manager;
	 GameEngine*	m_gameEngineOwnerBackPointer;
	 
	 std::unordered_map<std::string, Texture>		m_textures;
	 std::unordered_map<std::string, Animation>		m_animations;
	 std::unordered_map<std::string, std::string>	m_soundPaths;

	 std::unordered_map<std::string, int>			m_actionMap;

public:
	AbstractScene();
	virtual ~AbstractScene() = default;

	virtual void doAction() = 0;
};

