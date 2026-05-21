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

	//systems
	void sCollision();
	void sPhysics();

	enum class entityType {
		player,
		enemy,
		NPC,
		projectile,
		item,
		environment,
		other
	};

	void sSpawnEntity(entityType type , const vec2& position = {0 , 0});
	void sMovement();

	//getters
	std::shared_ptr<Entity> getCurrentSelectedEntity();
	std::unordered_map<std::string, std::function<void()>>& getActionMap();
	//setters
	void setCurrentSelectedEntity(const std::shared_ptr<Entity>& newEntity);

	void play() override;
	void sDoAction(const Action&) override;
	void updateInternals() override;
	void render() override;
};

