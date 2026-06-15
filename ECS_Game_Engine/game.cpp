/*
	
#include "game.h"
#include<fstream>
#include<random>

inline int random() {
	static std::random_device rd;

	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(1, 1000);

	return distr(gen);
}

void game::sMovement(double dt) {
	auto& entities = m_entity_manager.getEntities();
	for (auto& e : entities) {
		if (e->m_Transform && e->m_Velocity) {
			// Multiply velocity by Delta Time
			// New Pos = Old Pos + (Velocity * TimePassed)
			e->m_Transform->position.set_x(e->m_Transform->position.get_x() + (e->m_Velocity->velocity.get_x() * dt));
			e->m_Transform->position.set_y(e->m_Transform->position.get_y() + (e->m_Velocity->velocity.get_y() * dt));

			e->m_Transform->angle += 0.2f;
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

	//this is for bounding all the elements within the window
	//need to know the dimensions of the window
	int window_w{-1};
	int window_h{-1};

	SDL_GetWindowSize(m_window , &window_w , & window_h);

	for (auto& entity : entities) {
		if (entity->m_Velocity) {
			auto& vel_component = entity->m_Velocity;

			//condition for when the collider rectangle of the entity to not go out of bound of
			//the window
			//cap the poisiton x and the position y
			//cap the movement area for the entities that can move
			if (entity->m_Transform && entity->m_Velocity && entity->m_Shape) {

				auto& pos = entity->m_Transform->position;
				auto& vel = entity->m_Velocity->velocity;

				// Calculate half-width (radius) for collision
				float r = entity->m_Shape->w / 2.0f;

				// --- LEFT WALL ---
				if (pos.get_x() < r) {
					pos.set_x(r);               // 1. Clamp: Pull it out of the wall
					vel.set_x(vel.get_x() * -1);// 2. Bounce: Invert X velocity
				}
				// --- RIGHT WALL ---
				if (pos.get_x() > window_w - r) {
					pos.set_x(window_w - r);
					vel.set_x(vel.get_x() * -1);
				}
				// --- TOP WALL ---
				if (pos.get_y() < r) {
					pos.set_y(r);
					vel.set_y(vel.get_y() * -1);
				}
				// --- BOTTOM WALL ---
				if (pos.get_y() > window_h - r) {
					pos.set_y(window_h - r);
					vel.set_y(vel.get_y() * -1);
				}
			}
			
			if (auto input = entity->m_Input) {
				vel_component->velocity = { 0.0f , 0.0f };
				if (input->up) { vel_component->velocity.set_y(-vel_component->speed); }
				if (input->down) { vel_component->velocity.set_y(vel_component->speed); }
				if (input->left) { vel_component->velocity.set_x(-vel_component->speed); }
				if (input->right) { vel_component->velocity.set_x(vel_component->speed); }
			}
		}
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
void game::sRender() {
	auto entities = m_entity_manager.getEntities();

	for (auto& entity : entities) {
		if (entity->m_Shape && entity->m_Transform) {
			auto& shape = entity->m_Shape;
			auto& transform = entity->m_Transform;

			// FIX 1: Set the color BEFORE drawing!
			// Without this, you are drawing invisible black lines.
			SDL_SetRenderDrawColor(m_renderer, shape->r, shape->g, shape->b, 255);

			drawPolygon(
				transform->position.get_x(),        // Center X
				transform->position.get_y(),        // Center Y
				shape->w / 2.0f,                    // Radius (Half the width)
				shape->vertices,                    // Number of sides
				transform->angle                    // Rotation
			);
		}
	}
}
void game::sEnemySpawner() {
	if (m_current_frame - m_last_spawn_time >= m_enemyConfig.SI) {
		spawnEnemy();

		// Reset the timer
		m_last_spawn_time = m_current_frame;
	}
}

void game::sCollision() {
	auto& entities = m_entity_manager.getEntities();

	// Loop for collision checks
	for (auto& a : entities) {
		// We only care about collisions involving the Player for now
		// (Or loop all vs all if you want enemies bumping enemies)
		if (a->getTag() != "Player" && a->getTag() != "Enemy") continue;

		for (auto& b : entities) {
			// 1. Don't check self, and avoid double checking (A vs B, then B vs A)
			if (a == b) continue;
			if (a->getTag() == "Player" && b->getTag() == "Player") continue; // Don't check player vs player

			// 2. Check Overlap (Circle Collision)
			if (a->m_Shape && b->m_Shape && a->m_Transform && b->m_Transform) {

				vec2 posA = a->m_Transform->position;
				vec2 posB = b->m_Transform->position;
				vec2 diff = posA - posB; // Vector pointing from B to A

				double dist = diff.length();
				double radiusA = a->m_Shape->w / 2.0;
				double radiusB = b->m_Shape->w / 2.0;
				double overlap = (radiusA + radiusB) - dist;

				// COLLISION DETECTED
				if (overlap > 0) {

					// --- STEP 1: RESOLVE OVERLAP (Static Resolution) ---
					// Push them apart so they aren't stuck inside each other
					diff.normalize(); // Now 'diff' is the Normal Vector (n)

					// Move A half the overlap distance
					a->m_Transform->position.x += diff.x * (overlap / 2.0);
					a->m_Transform->position.y += diff.y * (overlap / 2.0);

					// Move B the other half
					b->m_Transform->position.x -= diff.x * (overlap / 2.0);
					b->m_Transform->position.y -= diff.y * (overlap / 2.0);


					// --- STEP 2: DYNAMIC RESOLUTION (The Bounce) ---
					// Only bounce if they have velocity
					if (a->m_Velocity && b->m_Velocity) {

						vec2& v1 = a->m_Velocity->velocity;
						vec2& v2 = b->m_Velocity->velocity;

						// Calculate Relative Velocity
						vec2 relativeVelocity = v1 - v2;

						// Calculate velocity along the normal (Dot Product)
						double velocityAlongNormal = relativeVelocity.x * diff.x + relativeVelocity.y * diff.y;

						// If velocities are separating, do nothing (they are already moving apart)
						if (velocityAlongNormal > 0) continue;

						// Calculate Restitution (Bounciness)
						// 1.0 = Perfect Bounce, 0.5 = Dull thud
						double e = 1.0;

						// Calculate Impulse Scalar
						double j = -(1 + e) * velocityAlongNormal;

						// Assuming equal mass for now (j /= 1/m1 + 1/m2 becomes j /= 2)
						j /= 2.0;

						// Apply Impulse
						vec2 impulse = diff * j;

						// Change velocities
						v1.x += impulse.x;
						v1.y += impulse.y;

						v2.x -= impulse.x;
						v2.y -= impulse.y;
					}
				}
			}
		}
	}
}

// 4. Helper Functions
void game::spawnPlayer(){
	auto entity = m_entity_manager.addEntity("Player");
	//the player is by default going to be a rectangle
	entity->m_Collider			= std::make_shared<cCollision>(m_playerConfig.h , m_playerConfig.w);
	entity->m_Input				= std::make_shared<cInput>();
	entity->m_Score				= std::make_shared<cScore>();
	entity->m_Transform			= std::make_shared<cTransform>(vec2(m_playerConfig.start_x , m_playerConfig.start_y) , 1.0 , 0.0);
	entity->m_Velocity			= std::make_shared<cVelocity>(vec2(0 , 0) , m_playerConfig.S);
	entity->m_Shape				= std::make_shared<cShape>(m_playerConfig.h, m_playerConfig.w , 255 , 255 , 255 , 4);//width height r g b total number of vertices

}
void game::spawnEnemy() {
	auto entity = m_entity_manager.addEntity("Enemy");

	int vertices = m_enemyConfig.VMIN + (rand() % (m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1));

	int window_w, window_h;
	SDL_GetWindowSize(m_window, &window_w, &window_h);

	float ex = (float)(rand() % (window_w - m_enemyConfig.SR * 2) + m_enemyConfig.SR);
	float ey = (float)(rand() % (window_h - m_enemyConfig.SR * 2) + m_enemyConfig.SR);

	float speed = m_enemyConfig.SMIN +
		(static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (m_enemyConfig.SMAX - m_enemyConfig.SMIN);

	float angle = (float)(rand() % 360) * (3.14159f / 180.0f); 
	vec2 velocityVec(cos(angle) * speed, sin(angle) * speed);

	entity->m_Transform = std::make_shared<cTransform>(vec2(ex, ey), 1.0f, 0.0);

	entity->m_Shape = std::make_shared<cShape>(
		m_enemyConfig.SR * 2.0f, m_enemyConfig.SR * 2.0f,
		m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB,
		vertices 
	);

	entity->m_Velocity = std::make_shared<cVelocity>(velocityVec, speed);

	entity->m_Collider = std::make_shared<cCollision>(m_enemyConfig.CR * 2, m_enemyConfig.CR *2);

	if (m_enemyConfig.L > 0) {
		entity->m_LifeSpan = std::make_shared<cLifeSpan>(m_enemyConfig.L);
	}
}
void game::spawnSmallEnemies(std::shared_ptr<Entity> entity){}
void game::spawnBullet(std::shared_ptr<Entity> entity, const vec2& mousePos){}
void game::spawnSpecialWeapon(std::shared_ptr<Entity> entity){}

void game::drawPolygon(double centerX, double centerY, double radius, int vertices, double rotation) {
	// 1. Create a vector of points (n + 1 to close the loop)
	std::vector<SDL_FPoint> points(vertices + 1);

	// 2. Calculate the angle step (in radians)
	float angleStep = (2.0f * 3.14159f) / vertices;

	for (int i = 0; i <= vertices; ++i) {
		// We add 'rotation' so the shape can spin
		double currentAngle = i * angleStep + rotation;

		points[i].x = centerX + radius * cosf(currentAngle);
		points[i].y = centerY + radius * sinf(currentAngle);
	}

	// 3. Draw the connected lines
	SDL_RenderLines(m_renderer, points.data(), (int)points.size());
}



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

	initConfigure(config_file);

	spawnPlayer();
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

			sEnemySpawner();

			m_current_frame++;	//for calculating frame related stuffs like that of the enemies that is being spawned
			sLifespan();
		}

		// --- Render ---
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderClear(m_renderer);
		sRender();
		SDL_RenderPresent(m_renderer);
	}
}
*/