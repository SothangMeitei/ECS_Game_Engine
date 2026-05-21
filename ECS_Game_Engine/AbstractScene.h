#pragma once
#include"ECS/EntityManager.h"
#include"Components/TexturesSpriteAnimation/cAnimation.h"
#include"Components/TexturesSpriteAnimation/cTexture.h"
#include<unordered_map>
#include"Components/Action.h"


class GameEngine;
class Action;

class AbstractScene
{
protected:
	 EntityManager	m_manager;
	 GameEngine*	m_gameEngineOwnerBackPointer;
	 
	 std::unordered_map<std::string, Texture>		m_textures;
	 std::unordered_map<std::string, Animation>		m_animations;
	 std::unordered_map<std::string, std::string>	m_soundPaths;

	 std::unordered_map<int, std::string>			m_actionMap;

public:
	AbstractScene() = default;
	virtual ~AbstractScene() = default;

	void registerAction(int inputKey, const std::string& actionName) {
		m_actionMap[inputKey] = actionName;
	}

	const std::unordered_map<int, std::string>& getActionMap() const {
		return m_actionMap;
	}

	virtual void sDoAction(const Action&) = 0;

	virtual void updateInternals() = 0;

	virtual void render() = 0;

	virtual void play() = 0;
};

