
#include "game.h"

void game::sMovement(){
	//how to do the movement
	//for each entity that has the position and the velocity component
	//update the position component of the entity based on the velocity 
	const auto & entities = m_entity_manager.getEntities();
	for (auto& e : entities) {
		if (e->m_Transform && e->m_Velocity) {
			e->m_Transform->position += e->m_Velocity->velocity;
		}
	}
}
void game::sUserInput(){

	const bool* keyboardStates = SDL_GetKeyboardState(nullptr);

	const auto & entities = m_entity_manager.getEntities();
	for (auto& e : entities) {
		if (e->m_Input) {
			//for now only the player will have the input component
			auto& input = e->m_Input; // Alias for cleaner code

			// Direct assignment handles both Press (true) and Release (false)
			input->up = keyboardStates[SDL_SCANCODE_W];
			input->down = keyboardStates[SDL_SCANCODE_S];
			input->left = keyboardStates[SDL_SCANCODE_A];
			input->right = keyboardStates[SDL_SCANCODE_D];
		}
	}
}
void game::sPhysics() {
	//change the various part of the physical world 
	//like that of the velocity of the entities and the rotational speeds

	//change the velocity depending on the input and the collision
	const auto & entities = m_entity_manager.getEntities();

	for (auto& entity : entities) {
		if (entity->m_Input && entity->m_Velocity) {
			auto& input = entity->m_Input;
			auto& vel = entity->m_Velocity;
			if (input->up) { entity->m_Velocity->velocity.set_y(-entity->m_Velocity->velocity.get_y()); }
			if (input->up)	{}
			if (input->up)	{}
			if (input->up)	{}
		}
	}
}
void game::sLifespan(){
	//see this is a reference and not a copy so this can be done in a fast manner
	auto entities = m_entity_manager.getEntities();
}
void game::sRender(){}
void game::sEnemySpawner(){}
void game::sCollision(){}

// 4. Helper Functions
void game::spawnPlayer(){}
void game::spawnEnemy(){}
void game::spawnSmallEnemies(std::shared_ptr<Entity> entity){}
void game::spawnBullet(std::shared_ptr<Entity> entity, const vec2& mousePos){}
void game::spawnSpecialWeapon(std::shared_ptr<Entity> entity){}

game::game(const std::string& config_file) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		//faliure to initailize the subsystem
		SDL_Log("Faliure to initialize the video subsystem %s", SDL_GetError());
		m_is_running = false;
	}
	if (!SDL_Init(SDL_INIT_AUDIO)) {
		//faliure to initailize the subsystem
		SDL_Log("Faliure to initialize the audio subsystem %s", SDL_GetError());
		m_is_running = false;
	}
	m_window = SDL_CreateWindow("Window", 400, 900, SDL_WINDOW_RESIZABLE);
	m_renderer = SDL_CreateRenderer(m_window, nullptr);
}

game::~game() {
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
}

void game::start() {
	while (m_is_running) {
		if (m_paused) {

		}
		else {

		}
	}
}