#pragma once
#include<SDL3/SDL.h>
#include<vector>
#include<string>
#include<unordered_map>
#include<memory>

//this will contain all the things that is to be same across all the game scenes

class AbstractScene;

class GameEngine {
private:
	SDL_Window*		m_gameWindow;
	SDL_Renderer*	m_gameRenderer;

	std::string		m_currentSceneName;
	std::unordered_map<std::string, std::shared_ptr<AbstractScene>>	m_mapSceneNameToSceneObject;

	bool m_isRunning;
	bool m_paused;

public:
	GameEngine();
	~GameEngine();

	void paused();
	void start();
	void quit();

	const std::string& getCurrentSceneName() const;
	SDL_Renderer* getRenderer() { return m_gameRenderer; }
	void changeScene(const std::string&);
	void addScene(const std::string& , std::shared_ptr<AbstractScene>);

	//systems

	void sUserInput();

};