#pragma once
#include<SDL3/SDL.h>
#include<vector>
#include<string>
#include"AbstractScene.h"
#include<unordered_map>

//this will contain all the things that is to be same across all the game scenes

class GameEngine {
private:
	SDL_Window* m_gameWindow;
	SDL_Renderer* m_gameRenderer;

	std::string m_currentSceneName;
	std::unordered_map<std::string, AbstractScene*> m_mapSceneNameToSceneObject;

	bool m_isRunning;
	bool m_paused;

public:
	GameEngine();
	~GameEngine();

	void pause();
	void run();
	void quit();

	std::string& getCurrentScene();
	void changeScene(std::string&);
	void addScene(std::string&);
	void suserInput();

};