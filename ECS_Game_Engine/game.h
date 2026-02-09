#pragma once
#include"ECS/EntityManager.h"
#include<SDL3/SDL.h>
#include"Components/vector.h"

struct PlayerConfig { int h , w , start_x , start_y; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };


class game
{
	//container for all that is in one game scene
private:
    //core engine components
    SDL_Window*                     m_window;
    SDL_Renderer*                   m_renderer;
    EntityManager                   m_entity_manager;
    bool                            m_is_running = true;
    bool                            m_paused = false;
    long long                       m_current_frame = 0;

    //input initial default values for the selected entities
    // 2. Configuration Data (Loaded from file)
    PlayerConfig        m_playerConfig;
    EnemyConfig         m_enemyConfig;
    BulletConfig        m_bulletConfig;

    //the systems
    void sMovement(double);
    void sUserInput();
    void sLifespan();
    void sRender();
    void sEnemySpawner();
    void sCollision();
    void sPhysics();

    // 4. Helper Functions
    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(std::shared_ptr<Entity> entity);
    void spawnBullet(std::shared_ptr<Entity> entity, const vec2& mousePos);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

    //configuration of the internal member struct
    void initConfigure(const std::string&);
public:
    game(const std::string&);
    ~game();

    void start();
};

