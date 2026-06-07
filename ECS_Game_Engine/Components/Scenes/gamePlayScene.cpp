#include "gamePlayScene.h"
#include"../../GameEngine.h"
#include"../../QuadTree/QuadTree.h"
#include<random>

inline int random(int start , int end) {
    static std::random_device rd;

    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(start, end);

    return distr(gen);
}

gamePlayScene::gamePlayScene(GameEngine* engine , const std::string& configFile) {
    m_gameEngineOwnerBackPointer = engine;
    //register all the action requried in this engine
    registerAction(SDL_SCANCODE_W, "up");
    registerAction(SDL_SCANCODE_S, "down");
    registerAction(SDL_SCANCODE_A, "left");
    registerAction(SDL_SCANCODE_D, "right");

    registerAction(SDL_SCANCODE_ESCAPE, "quit_to_main_menu");

    // --- KEY DOWN ACTIONS (START) ---
    m_startAction["up"] = [this]() {
        if (m_currentSelectedEntity && m_currentSelectedEntity->m_Input)
            m_currentSelectedEntity->m_Input->up = true;
            
        SDL_Log("UP UP UP UP UP UP movement done , Character w pressed\n");
        };
    m_startAction["down"] = [this]() {
        if (m_currentSelectedEntity && m_currentSelectedEntity->m_Input)
            m_currentSelectedEntity->m_Input->down = true;
        SDL_Log("DOWN DOWN DOWN DOWN movement done , Character s pressed\n");
        };
    m_startAction["left"] = [this]() {
        if (m_currentSelectedEntity && m_currentSelectedEntity->m_Input)
            m_currentSelectedEntity->m_Input->left = true;
        SDL_Log("LEFT LEFT LEFT LEFT movement done , Character a pressed\n");
        };
    m_startAction["right"] = [this]() {
        if (m_currentSelectedEntity && m_currentSelectedEntity->m_Input)
            m_currentSelectedEntity->m_Input->right = true;
        SDL_Log("RIGHT RIGHT RIGHT RIGHT movement done , Character f pressed\n");
        };

    // Toggle is a discrete action. It only happens on key DOWN.
    m_startAction["toggle"] = [this]() {
        // Logic to cycle m_currentSelectedEntity to the next entity
        };
    m_startAction["quit_to_main_menu"] = [this]() {
        SDL_Log("Escape pressed. Returning to Menu.");

        // Use the back-pointer to tell the engine to swap the active scene
        if (m_gameEngineOwnerBackPointer) {
            // NOTE: Make sure the string "Menu" exactly matches the string 
            // you used in main.cpp when calling gameEngine1.addScene("Menu", menuScene);
            m_gameEngineOwnerBackPointer->changeScene("Menu");
        }
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
        std::shared_ptr<Entity> newEntity = m_manager.addEntity(event->type);   //this type is for the type of entity that is specified in the event e
        });





    //load up the assets
    m_assetManager.addTexture("mainPlayerTexture" , "./GameAssets/sprite_image.bmp", m_gameEngineOwnerBackPointer->getRenderer());


    //spawn the in game entities for the first time the player enters the scene
    //populate the current scene
    
    auto p1 = spawnPlayer({ 100.0f, 100.0f });
    m_currentSelectedEntity = p1;
    m_Camera.setTarget(m_currentSelectedEntity);
    //for (int i = 0; i < 10; ++i) {
    //    spawnEnemy(vec2(random(0 , windowDimension[1]) , random(0 , windowDimension[1])));  //spawn 10 enemies randomly in bounded by the current screen
    //}
    p1->m_Animation->states["Running Up"]       = { 8 , 50 , { 0, 2432, 64, 64 } };
    p1->m_Animation->states["Running Left"]     = { 8 , 50 , { 0, 2496, 64, 64 } };
    p1->m_Animation->states["Running Down"]    = { 8 , 50 , { 0, 2560, 64, 64 } };
    p1->m_Animation->states["Running Right"]     = { 8 , 50 , { 0, 2624, 64, 64 } };

    p1->m_Animation->states["Idle Up"] = { 8 , 100 , { 0, 256, 64, 64 } };
    p1->m_Animation->states["Idle Left"] = { 8 , 100 , { 0, 320, 64, 64 } };
    p1->m_Animation->states["Idle Down"] = { 8 , 100 , { 0, 384, 64, 64 } };
    p1->m_Animation->states["Idle Right"] = { 8 , 100 , { 0, 448, 64, 64 } };

    p1->m_Animation->changeState("Idle Down");


    spawnEnemy({400 , 400});
    spawnEnemy({500 , 500});
    spawnEnemy({200 , 200});
}


//systems
void gamePlayScene::sCollision() {
    auto entities = m_manager.getEntities();

    //at the start of each of the frame construct the quad tree
    const std::array<int, 2> currentWindowWidthHeight{ m_gameEngineOwnerBackPointer->getWindowDimension() };

    QuadTree collisionTree(0
        , AABB_Bounds{ 0, 0, static_cast<float>(currentWindowWidthHeight[0]) , static_cast<float>(currentWindowWidthHeight[1]) });

    //insert every entity into the tree , this will construct the quad tree to with all the nodes , to the leaf
    for (auto& e : entities) {
        if (e->m_Collider && e->m_Transform) {
            collisionTree.insert(e);
        }
    }

    //Resolve Collisions , poll for each entity , the nearby entities and then
    //run n ^ n time algorithm on all the pair of entities with in this bound of this current quad and resolve the collision
    for (auto& e1 : entities) {
        if (!e1->m_Collider || !e1->m_Transform || !e1->m_Velocity) continue;

        AABB_Bounds searchBounds = {
            e1->m_Transform->position.get_x(),
            e1->m_Transform->position.get_y(),
            (float)e1->m_Collider->w,
            (float)e1->m_Collider->h
        };

        // Get ONLY the entities in the same quadrants
        std::vector<std::shared_ptr<Entity>> nearbyEntities;
        collisionTree.retrieve(nearbyEntities, searchBounds);

        for (auto& e2 : nearbyEntities) {
            if (e1 == e2 || !e2->m_Collider || !e2->m_Transform) continue;

            std::shared_ptr<cCollision> e1Collider = e1->m_Collider;
            std::shared_ptr<cCollision> e2Collider = e2->m_Collider;

            // 1. Calculate the exact center points
            float c1x = e1->m_Transform->position.get_x() + (e1Collider->w / 2.0f);
            float c1y = e1->m_Transform->position.get_y() + (e1Collider->h / 2.0f);
            float c2x = e2->m_Transform->position.get_x() + (e2Collider->w / 2.0f);
            float c2y = e2->m_Transform->position.get_y() + (e2Collider->h / 2.0f);

            // 2. Calculate the distance between centers (The Vector from e2 to e1)
            float dx = c1x - c2x;
            float dy = c1y - c2y;

            // 3. Calculate the absolute distance
            float abs_dx = std::abs(dx);
            float abs_dy = std::abs(dy);

            // 4. Calculate the minimum distance needed to NOT be colliding 
            // , ei the seperation of the widhts and the heigths of the two boxes
            float min_dist_x = (e1Collider->w / 2.0f) + (e2Collider->w / 2.0f);
            float min_dist_y = (e1Collider->h / 2.0f) + (e2Collider->h / 2.0f);

            // 5. Calculate Penetration Depth (Overlap)
            // If overlap is negative, they are separated.
            float overlapX = min_dist_x - abs_dx;
            float overlapY = min_dist_y - abs_dy;

            // Overlap must exist on BOTH axes to be a collision. 
            // If either overlap is 0 or less, skip.
            if (overlapX <= 0 || overlapY <= 0) continue;

            //collision happened , now resolve the collision
            if (overlapX < overlapY) {
                if (dx > 0) {
                    e1->m_Transform->position.set_x(e1->m_Transform->position.get_x() + overlapX);
                }
                else {
                    e1->m_Transform->position.set_x(e1->m_Transform->position.get_x() - overlapX);
                }
                e1->m_Velocity->velocity.set_x(0);
            }
            else {
                if (dy > 0) {
                    e1->m_Transform->position.set_y(e1->m_Transform->position.get_y() + overlapY);
                }
                else {
                    e1->m_Transform->position.set_y(e1->m_Transform->position.get_y() - overlapY);
                }
                e1->m_Velocity->velocity.set_y(0);
            }
        }
    }
}
void gamePlayScene::sPhysics(){}
void gamePlayScene::sInput() {
    auto& entities{ m_manager.getEntities() };

    for (auto& e : entities) {
        if (e->m_Input) {
            //so this is for the input resolution of the entity
            //this is for all the input resolution of the game
            auto inputComponent{ e->m_Input };

            e->m_Velocity->velocity.set_x(0.0f);
            e->m_Velocity->velocity.set_y(0.0f);


            if (inputComponent->up) { 
                e->m_Velocity->velocity.set_y(-e->m_Velocity->speed);
                e->m_Animation->changeState("Running Up");
            }
            if (inputComponent->down) { 
                e->m_Velocity->velocity.set_y(e->m_Velocity->speed); 
                e->m_Animation->currentAnimName = "Running Down";
            }
            if (inputComponent->right) { 
                e->m_Velocity->velocity.set_x(e->m_Velocity->speed); 
                e->m_Animation->currentAnimName = "Running Right";
            }
            if (inputComponent->left) {
                e->m_Velocity->velocity.set_x(-e->m_Velocity->speed); 
                e->m_Animation->currentAnimName = "Running Left";
            }
        }
    }
}

void gamePlayScene::sMovement() {
    auto& entities = m_manager.getEntities();

    for (auto& e : entities) {

        // 1. Apply physical movement
        if (e->m_Transform && e->m_Velocity) {
            e->m_Transform->position += e->m_Velocity->velocity;
        }

        // 2. Control the Animation State based on Velocity
        if (e->m_Animation && e->m_Velocity) {
            float vx = e->m_Velocity->velocity.get_x();
            float vy = e->m_Velocity->velocity.get_y();

            std::string currentAnim = e->m_Animation->currentAnimName;
            std::string nextAnim = currentAnim;

            // --- IS THE ENTITY MOVING? ---
            if (vx > 0)      nextAnim = "Running Right";
            else if (vx < 0) nextAnim = "Running Left";
            else if (vy > 0) nextAnim = "Running Down";
            else if (vy < 0) nextAnim = "Running Up";

            // --- IS THE ENTITY STOPPED? ---
            else {
                // We use the LAST KNOWN running state to figure out which way to idle!
                if (currentAnim == "Running Right") nextAnim = "Idle Right";
                else if (currentAnim == "Running Left") nextAnim = "Idle Left";
                else if (currentAnim == "Running Down") nextAnim = "Idle Down";
                else if (currentAnim == "Running Up") nextAnim = "Idle Up";
            }

            // Apply the change safely!
            e->m_Animation->changeState(nextAnim);
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

//spawning functions
std::shared_ptr<Entity> gamePlayScene::spawnPlayer(const vec2& position) {
    auto player = m_manager.addEntity("Player");    //input taken by the addEntity is the type of the entity that is added

    player->m_Transform     = std::make_shared<cTransform>(position, 1.0f, 0.0f);
    player->m_Velocity      = std::make_shared<cVelocity>(vec2(0.0f, 0.0f) , 0.1);
    player->m_Input         = std::make_shared<cInput>();
    player->m_Health        = std::make_shared<cHealth>(100);
    player->m_Shape         = std::make_shared<cShape>(64, 64, 0, 0, 255, 4);
    player->m_Collider      = std::make_shared<cCollision>(40 , 40);
    player->m_Animation     = std::make_shared<cAnimation>("mainPlayerTexture");
    player->m_LifeSpan      = nullptr;
    player->m_TextOutput    = nullptr;

    return player;
}
std::shared_ptr<Entity> gamePlayScene::spawnEnemy(const vec2& position) {
    auto enemy = m_manager.addEntity("Enemy");

    enemy->m_Transform      = std::make_shared<cTransform>(position, 1.0f, 0.0f);
    enemy->m_Velocity       = std::make_shared<cVelocity>(vec2(0.0f, 0.0f) , 0.1f);
    enemy->m_Health         = std::make_shared<cHealth>(50);
    enemy->m_Shape          = std::make_shared<cShape>(64, 64, 255, 0, 0, 4);
    enemy->m_Collider       = std::make_shared<cCollision>(64, 64);
    enemy->m_Input          = nullptr;
    enemy->m_LifeSpan       = nullptr;
    enemy->m_TextOutput     = nullptr;

    return enemy;
}
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
    m_manager.update();
    sInput();
	sMovement();
	sCollision();
	sPhysics();
    sAnimation(deltaTime);
    m_internalEventBus.resolveEventsFromEventQueue();
}

//helper for the renderer system
inline void drawPolygon(double centerX, double centerY, double radius, int vertices, double rotation, std::shared_ptr<cShape> shape , SDL_Renderer* m_renderer) {
    std::vector<SDL_FPoint> points(vertices + 1);
    float angleStep = (2.0f * 3.14159f) / vertices;

    SDL_SetRenderDrawColor(m_renderer, shape->r, shape->g, shape->b, 255);
    for (int i = 0; i <= vertices; ++i) {
        // We add 'rotation' so the shape can spin
        double currentAngle = i * angleStep + rotation;

        points[i].x = centerX + radius * cosf(currentAngle);
        points[i].y = centerY + radius * sinf(currentAngle);
    }
    SDL_RenderLines(m_renderer, points.data(), (int)points.size());
}
void gamePlayScene::render() {
    SDL_Renderer* renderer = m_gameEngineOwnerBackPointer->getRenderer();

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

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
        if (entity->m_Transform && !entity->m_Animation && entity->m_Collider) {
            //render the shape of the entity
            auto& collider = entity->m_Collider;
            auto& position = entity->m_Transform->position;
            drawPolygon(
                position.get_x() + collider->w / 2,
                position.get_y() + collider->h / 2,
                collider->w / 2,
                4,
                95.0f,     //this rotation is in radiants
                entity->m_Shape,
                m_gameEngineOwnerBackPointer->getRenderer()
            );
        }
    }
    SDL_RenderPresent(renderer);
}
void gamePlayScene::play(float deltaTime) {
	updateInternals(deltaTime);
	render();
}