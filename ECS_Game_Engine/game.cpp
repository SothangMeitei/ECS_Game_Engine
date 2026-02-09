
#include "game.h"
#include<fstream>

void game::sMovement(double dt) {
	auto& entities = m_entity_manager.getEntities();
	for (auto& e : entities) {
		if (e->m_Transform && e->m_Velocity) {
			// Multiply velocity by Delta Time
			// New Pos = Old Pos + (Velocity * TimePassed)
			e->m_Transform->position.set_x(e->m_Transform->position.get_x() + (e->m_Velocity->velocity.get_x() * dt));
			e->m_Transform->position.set_y(e->m_Transform->position.get_y() + (e->m_Velocity->velocity.get_y() * dt));
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

			vel->velocity = { 0.0f , 0.0f };

			//and the velocity is set depending on the seed of the entity
			if (input->up)		{ vel->velocity.set_y(-vel->speed); }
			if (input->down)	{ vel->velocity.set_y(vel->speed); }
			if (input->left)	{ vel->velocity.set_x(-vel->speed); }
			if (input->right)	{ vel->velocity.set_x(vel->speed); }
		}
		std::cout << "debug the position of the entity: ";
		std::cout << entity->m_Transform->position.get_x() << " " << entity->m_Transform->position.get_y() << "\n";
	}
}
void game::sLifespan(){
	//see this is a reference and not a copy so this can be done in a fast manner
	auto entities = m_entity_manager.getEntities();

	for (auto& entity : entities) {
		if (entity->m_LifeSpan) {
			if (entity->m_LifeSpan->total_frames <= 0) {
				entity->destroy();
			}
			entity->m_LifeSpan->total_frames--;
		}
	}
}
void game::sRender(){
	//start drawing all the things that can be drawn
	//if it has a shape it can be drawn
	auto entities = m_entity_manager.getEntities();

	for (auto& entity : entities) {
		//for now all the shapes are just going to be rectangles
		if (entity->m_Shape && entity->m_Transform) {
			auto& shape = entity->m_Shape;
			auto& transform = entity->m_Transform;
			//now how to render this rectangle onto the screen
			//a fill rectangle
			SDL_FRect rect;

			rect.w = shape->w;
			rect.h = shape->h;
			rect.x = (transform->position.get_x()) - (rect.w / 2);
			rect.y = (transform->position.get_y()) - (rect.h / 2);

			SDL_RenderFillRect(m_renderer, &rect);
			SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255); // White border
			SDL_RenderRect(m_renderer, &rect);
		}
	}
}
void game::sEnemySpawner(){}
void game::sCollision(){}

// 4. Helper Functions
void game::spawnPlayer(){}
void game::spawnEnemy(){}
void game::spawnSmallEnemies(std::shared_ptr<Entity> entity){}
void game::spawnBullet(std::shared_ptr<Entity> entity, const vec2& mousePos){}
void game::spawnSpecialWeapon(std::shared_ptr<Entity> entity){}

void game::initConfigure(const std::string& config_file) {
	std::ifstream fin(config_file);

	if (!fin) {
		std::cerr << "Could not load config file: " << config_file << std::endl;
		return;
	}

	std::string type;

	// Loop through the file until we hit the end
	while (fin >> type) {

		if (type == "Player") {
			// Read Player Config: Shape Radius, Collision Radius, Speed, etc.
			// Example File Line: Player 32 32 5.0 1280 720 
			// Matches struct: int h, w, start_x, start_y; float S;
			// NOTE: The order below must match your text file EXACTLY.

			fin >> m_playerConfig.w        // Width
				>> m_playerConfig.h        // Height
				>> m_playerConfig.start_x  // Start X
				>> m_playerConfig.start_y  // Start Y
				>> m_playerConfig.S;       // Speed (Max Speed)
		}
		else if (type == "Enemy") {
			// Matches struct: SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI, SMIN, SMAX
			fin >> m_enemyConfig.SR >> m_enemyConfig.CR
				>> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB
				>> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX
				>> m_enemyConfig.L >> m_enemyConfig.SI
				>> m_enemyConfig.SMIN >> m_enemyConfig.SMAX;
		}
		else if (type == "Bullet") {
			// Matches struct: SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L, S
			fin >> m_bulletConfig.SR >> m_bulletConfig.CR
				>> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB
				>> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB
				>> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L
				>> m_bulletConfig.S;
		}
	}
	fin.close();
}
game::game(const std::string& config_file) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Faliure to initialize the video subsystem %s", SDL_GetError());
		m_is_running = false;
	}
	if (!SDL_Init(SDL_INIT_AUDIO)) {
		SDL_Log("Faliure to initialize the audio subsystem %s", SDL_GetError());
		m_is_running = false;
	}
	m_window	= SDL_CreateWindow("Window", 900, 400, SDL_WINDOW_RESIZABLE);
	m_renderer	= SDL_CreateRenderer(m_window, nullptr);

	auto player = m_entity_manager.addEntity();

	player->m_Transform = std::make_shared<cTransform>(vec2(200.0f, 200.0f), 1.0f, 0.0f);
	player->m_Shape = std::make_shared<cShape>(32.0f, 32.0f, 255, 0, 0);
	player->m_Input = std::make_shared<cInput>();
	player->m_Velocity = std::make_shared<cVelocity>(vec2(0.0f, 0.0f), 1.0f);
}

game::~game() {
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
}

void game::start() {
	// Frequency of the CPU timer
	Uint64 perfFreq = SDL_GetPerformanceFrequency();
	Uint64 lastTime = SDL_GetPerformanceCounter();

	while (m_is_running) {
		// 1. Calculate Delta Time (dt) in Seconds
		Uint64 now = SDL_GetPerformanceCounter();
		double dt = (double)((now - lastTime) / (double)perfFreq);
		lastTime = now;

		// --- Polling (Must be First) ---
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) m_is_running = false;
		}

		if (!m_paused) {
			m_entity_manager.update();

			// System Order: Input -> Physics -> Movement
			sUserInput();
			sPhysics();
			sMovement(dt); // Pass dt here!

			sCollision();
			sLifespan();
		}

		// --- Render ---
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderClear(m_renderer);
		sRender();
		SDL_RenderPresent(m_renderer);
	}
}