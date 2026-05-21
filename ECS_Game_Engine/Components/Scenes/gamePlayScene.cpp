#include "gamePlayScene.h"
gamePlayScene::gamePlayScene(const std::string& configFile) {

    // --- KEY DOWN ACTIONS (START) ---
    m_startAction["up"] = [this]() {
        if (m_currentSelectedEntity && m_currentSelectedEntity->m_Input)
            m_currentSelectedEntity->m_Input->up = true;
        };
    m_startAction["down"] = [this]() {
        if (m_currentSelectedEntity && m_currentSelectedEntity->m_Input)
            m_currentSelectedEntity->m_Input->down = true;
        };
    m_startAction["left"] = [this]() {
        if (m_currentSelectedEntity && m_currentSelectedEntity->m_Input)
            m_currentSelectedEntity->m_Input->left = true;
        };
    m_startAction["right"] = [this]() {
        if (m_currentSelectedEntity && m_currentSelectedEntity->m_Input)
            m_currentSelectedEntity->m_Input->right = true;
        };

    // Toggle is a discrete action. It only happens on key DOWN.
    m_startAction["toggle"] = [this]() {
        // Logic to cycle m_currentSelectedEntity to the next entity
        };

    // --- KEY UP ACTIONS (END) ---
    m_endAction["up"] = [this]() {
        if (m_currentSelectedEntity && m_currentSelectedEntity->m_Input)
            m_currentSelectedEntity->m_Input->up = false;
        };
    m_endAction["down"] = [this]() {
        if (m_currentSelectedEntity && m_currentSelectedEntity->m_Input)
            m_currentSelectedEntity->m_Input->down = false;
        };
    m_endAction["left"] = [this]() {
        if (m_currentSelectedEntity && m_currentSelectedEntity->m_Input)
            m_currentSelectedEntity->m_Input->left = false;
        };
    m_endAction["right"] = [this]() {
        if (m_currentSelectedEntity && m_currentSelectedEntity->m_Input)
            m_currentSelectedEntity->m_Input->right = false;
        };
}

//systems
void gamePlayScene::sCollision(){
	//for all the entities that have a collider component do the collision resolution here
	//this is for the resolution of the collision only and not the physics which will be concerning with the 
	//velocities and positions and acceleration and stuffs that is not related to collision and thus this collision will be
	//some form of subpart of physics that is to be specialized for the purpose of it being so important
	auto entities = m_manager.getEntities();
	for (auto& e : entities) {
        if (e->m_Collider) {

        }
	}
}
void bindEntitesToScreen(int width , int heigth){}
void gamePlayScene::sPhysics(){}
void gamePlayScene::sSpawnEntity(entityType type, const vec2& position) {}
void gamePlayScene::sMovement(){
    auto& entities = m_manager.getEntities();
    for (auto& e : entities) {
        
        //for the setting of the velocity that is dependent on the input 
        //other part such as the physics and the collision will be dependent on the settings made by this movement
        if (e->m_Transform && e->m_Velocity) {
            e->m_Transform->position += e->m_Velocity->velocity;
        }
    }
}

//getters
std::shared_ptr<Entity> gamePlayScene::getCurrentSelectedEntity() {
	return m_currentSelectedEntity;
}
std::unordered_map<std::string, std::function<void()>>& gamePlayScene::getActionMap() {
	return m_startAction;
}
//setters
void gamePlayScene::setCurrentSelectedEntity(const std::shared_ptr<Entity>& newEntity){
	m_currentSelectedEntity = newEntity;
}
//overrides
void gamePlayScene::sDoAction(const Action& action) {
	const std::string& name{ action.get_name() };
	const std::string& type{ action.get_type() };

	// 1. Guard clause: Ensure we actually have an action mapped to prevent crashes
	if (type == "START") {
		if (m_startAction.find(name) != m_startAction.end()) {
			m_startAction[name]();
		}
	}
	else if (type == "END") {
		if (m_endAction.find(name) != m_endAction.end()) {
			m_endAction[name]();
		}
	}
}
void gamePlayScene::updateInternals() {
	sMovement();
	sCollision();
	sPhysics();
}
void gamePlayScene::render() {}
void gamePlayScene::play() {
	updateInternals();
	render();
}