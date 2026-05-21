#pragma once
#include"../../AbstractScene.h"
#include<functional>

class gamePlayScene: public AbstractScene
{
private:
	std::shared_ptr<Entity> m_currentSelectedEntity;
	std::unordered_map<std::string, std::function<void()>> m_startAction;
	std::unordered_map<std::string, std::function<void()>> m_endAction;
	
public:
	gamePlayScene(const std::string& configFile);

	enum class entityType {
		player,
		enemy,
		NPC,
		projectile,
		item,
		environment,
		other
	};

	//systems
	void sCollision();
	void sPhysics();
	void sMovement();
	void sPlayAudio(const std::string& audioFileName);
	void sAnimation(float deltaTime);

	//helper functions for the spawning of the in game entity types
	std::shared_ptr<Entity> spawnPlayer(const vec2& position);
	std::shared_ptr<Entity> spawnEnemy(const vec2& position);
	std::shared_ptr<Entity> spawnBullet(const vec2& position, const vec2& velocity, int damage, float lifespanSeconds);

	//getters
	std::shared_ptr<Entity> getCurrentSelectedEntity();
	std::unordered_map<std::string, std::function<void()>>& getActionMap();
	//setters
	void setCurrentSelectedEntity(const std::shared_ptr<Entity>& newEntity);

	//overrides
	void play(float) override;
	void sDoAction(const Action&) override;
	void updateInternals(float) override;
	void render() override;
};

