#include "gamePlayScene.h"
#include"../../GameEngine.h"

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

    //functions for the internal event bus event resolution 
    //for the damage to entity
    m_internalEventBus.subscribe(eventTypes::damageEvent, [this](const event* e) {
        //damage to the damage reciever is already happned so one is to resolve this event 
        //for the health resolution
        const DamageEvent* damage = dynamic_cast<const DamageEvent*>(e);
        if (damage->damageReciever && damage->damageReciever->m_Health) {
            damage->damageReciever->m_Health->healthValue -= damage->damageAmount;
            if (damage->damageReciever->m_Health->healthValue <= 0) damage->damageReciever->destroy();
        }
        });

    m_internalEventBus.subscribe(eventTypes::damageEvent, [this](const event* e) {
        //for the animation resolution , by setting the animation state
        const DamageEvent* damage = dynamic_cast<const DamageEvent*>(e);
        std::shared_ptr<Entity> victim{ damage->damageReciever };
        if (victim && victim->m_Animation) {
            victim->m_Animation->changeState("flinch");
        }
        });

    m_internalEventBus.subscribe(eventTypes::spawnEntityEvent, [this](const event* e) {
        const SpawnEntityEvent* event = dynamic_cast<const SpawnEntityEvent*>(e);
        //create an entity from this event and then push it to the entities vector
        std::shared_ptr<Entity> newEntity = m_manager.addEntity(event->type);
        });


    //spawn the in game entities for the first time the player enters the scene
    //populate the current scene
    
    //std::array<int, 2> windowDimension{ m_gameEngineOwnerBackPointer->getWindowDimension() };
    //spawnPlayer(vec2(windowDimension[0] / 2 , windowDimension[1] / 2));
    //spawnEnemy(vec2(windowDimension[0], windowDimension[1]));
}





//systems
void gamePlayScene::sCollision(){
	//for all the entities that have a collider component do the collision resolution here
	//this is for the resolution of the collision only and not the physics which will be concerning with the 
	//velocities and positions and acceleration and stuffs that is not related to collision and thus this collision will be
	//some form of subpart of physics that is to be specialized for the purpose of it being so important
	auto entities = m_manager.getEntities();
	for (auto& e1 : entities) {
        if (!e1->m_Collider) continue;
        for (auto& e2 : entities) {
            //O(n^2) time very heavy and clunky , need to implement the quad tree data structure
            //make it in the performant array stucture for the cache friendly nature
            
        }
	}
}
void bindEntitesToScreen(int width , int heigth){}
void gamePlayScene::sPhysics(){}

void gamePlayScene::sMovement(){
    auto& entities = m_manager.getEntities();
    for (auto& e : entities) {
        if (e->m_Transform && e->m_Velocity) {
            e->m_Transform->position += e->m_Velocity->velocity;
        }
    }
}
void gamePlayScene::sPlayAudio(const std::string& audioName){
    const std::string& path{ m_soundPaths[audioName] };

}
void gamePlayScene::sAnimation(float deltaTime) {   //delta time is the time taken for each frame for the game
    for (const auto& entity : m_manager.getEntities()) {
        if (entity->m_Animation) {
            auto& anim = entity->m_Animation;
            const AnimState& currentStateMath = anim->states[anim->currentAnimName];
            anim->timeElapsed += deltaTime;

            if (anim->timeElapsed >= currentStateMath.frameDuration) {
                anim->timeElapsed -= currentStateMath.frameDuration;
                anim->currentFrame = (anim->currentFrame + 1) % currentStateMath.frameCount;
            }
        }
    }
}



//helper functions for the spawning of the different entity types in the game
// -----------------------------------------------------------------
// SPAWN: PLAYER
// -----------------------------------------------------------------
std::shared_ptr<Entity> gamePlayScene::spawnPlayer(const vec2& position) {
    auto player = m_manager.addEntity("Player");

    player->m_Transform     = std::make_shared<cTransform>(position, 1.0f, 0.0f);
    player->m_Velocity      = std::make_shared<cVelocity>(vec2(0.0f, 0.0f));
    player->m_Input         = std::make_shared<cInput>();
    player->m_Health        = std::make_shared<cHealth>(100);
    player->m_Shape         = std::make_shared<cShape>(64, 64, 0, 0, 255, 4);
    player->m_Collider      = std::make_shared<cCollision>(64 , 64);
    player->m_Animation     = std::make_shared<cAnimation>("mainPlayerTexture");
    player->m_LifeSpan      = nullptr; 
    player->m_TextOutput    = nullptr;

    return player;
}

// -----------------------------------------------------------------
// SPAWN: ENEMY
// -----------------------------------------------------------------
std::shared_ptr<Entity> gamePlayScene::spawnEnemy(const vec2& position) {
    auto enemy = m_manager.addEntity("Enemy");

    enemy->m_Transform      = std::make_shared<cTransform>(position, 1.0f, 0.0f);
    enemy->m_Velocity       = std::make_shared<cVelocity>(vec2(0.0f, 0.0f));
    enemy->m_Health         = std::make_shared<cHealth>(50);
    enemy->m_Shape          = std::make_shared<cShape>(64, 64, 255, 0, 0, 4);
    enemy->m_Collider       = std::make_shared<cCollision>(64, 64);
    enemy->m_Input          = nullptr;
    enemy->m_LifeSpan       = nullptr;
    enemy->m_TextOutput     = nullptr;

    return enemy;
}

// -----------------------------------------------------------------
// SPAWN: BULLET
// -----------------------------------------------------------------
std::shared_ptr<Entity> gamePlayScene::spawnBullet(const vec2& position, const vec2& velocity, int damage, float lifespanSeconds) {
    auto bullet = m_manager.addEntity("Bullet");

    bullet->m_Transform = std::make_shared<cTransform>(position, 1.0f, 0.0f);
    bullet->m_Velocity = std::make_shared<cVelocity>(velocity);
    bullet->m_Shape = std::make_shared<cShape>(10, 10, 255, 255, 0, 4);
    bullet->m_Health = std::make_shared<cHealth>(damage);
    bullet->m_LifeSpan = std::make_shared<cLifeSpan>(lifespanSeconds);
    bullet->m_Input = nullptr;
    bullet->m_TextOutput = nullptr;

    return bullet;
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
void gamePlayScene::updateInternals(float deltaTime) {
	sMovement();
	sCollision();
	sPhysics();
    sAnimation(deltaTime);
}
void gamePlayScene::render() {
    for (auto& entity : m_manager.getEntities()) {
        if (entity->m_Transform && entity->m_Animation) {
            auto& anim = entity->m_Animation;
            SDL_Texture* tex = m_assetManager.getTexture(anim->textureID);

            const AnimState& stateMath = anim->states[anim->currentAnimName];

            // Calculate the current frame's rectangle on the sprite sheet
            SDL_FRect sourceRect = stateMath.startRect;
            sourceRect.x += (anim->currentFrame * sourceRect.w); // Shift right by N frames

            SDL_FRect destRect = { entity->m_Transform->position.x, entity->m_Transform->position.y, sourceRect.w, sourceRect.h };

            SDL_RenderTexture(m_gameEngineOwnerBackPointer->getRenderer(), tex, &sourceRect, &destRect);
        }
    }
}
void gamePlayScene::play(float deltaTime) {
	updateInternals(deltaTime);
	render();
}