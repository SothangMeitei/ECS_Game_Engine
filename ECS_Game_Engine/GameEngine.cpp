#pragma once
#include"GameEngine.h"
#include"AbstractScene.h"
#include"Components/Action.h"

GameEngine::GameEngine(){
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Faliure to initialize the video subsystem %s", SDL_GetError());
		m_isRunning = false;
	}
	if (!SDL_Init(SDL_INIT_AUDIO)) {
		SDL_Log("Faliure to initialize the audio subsystem %s", SDL_GetError());
		m_isRunning = false;
	}
	m_gameWindow = SDL_CreateWindow("Window", 900, 400, SDL_WINDOW_RESIZABLE);
	if (!m_gameWindow) {
		SDL_Log("Faliur to initialize the window using SDL\n");
		m_isRunning = false;
		return;
	}
	m_gameRenderer = SDL_CreateRenderer(m_gameWindow, nullptr);
	if (!m_gameRenderer) {
		SDL_Log("Faliur to initialize the renderer\n");
		m_isRunning = false;
		return;
	}
}
GameEngine::~GameEngine(){
	SDL_DestroyRenderer(m_gameRenderer);
	SDL_DestroyWindow(m_gameWindow);
}



void GameEngine::paused() { m_paused = !m_paused; }
void GameEngine::start() { 
	while (m_isRunning) {
		sUserInput();
		m_mapSceneNameToSceneObject[getCurrentSceneName()]->play();
	}
}
void GameEngine::quit(){m_isRunning = false;}

void GameEngine::addScene(const std::string& newSceneName , std::shared_ptr<AbstractScene> scene){
	m_mapSceneNameToSceneObject[newSceneName] = scene;
}
const std::string& GameEngine::getCurrentSceneName() const {
	return m_currentSceneName;
}
void GameEngine::changeScene(const std::string& newSceneName) {
	m_currentSceneName = newSceneName;
}


//this takes in the user input and then generates the correct action
//for the current scene , if there is not action for the current scene 
//no action is generated and thus no action is performed in the current scene for one frame
void GameEngine::sUserInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		//try handelingg all the window closing and the pausing information right here

		if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
			std::string actionType{ (event.type == SDL_EVENT_KEY_DOWN) ? "START" : "END" };

			auto& currentScene = m_mapSceneNameToSceneObject[getCurrentSceneName()];
			auto& currentSceneInputMap{ currentScene->getActionMap() };
			int currentKey = event.key.key;

			// if this current scene has a mapping for this current key press
			//create the action for this and then sent to the current scene
			if (currentSceneInputMap.find(currentKey) != currentSceneInputMap.end()) {
				const std::string& actionName = currentSceneInputMap.at(currentKey);
				currentScene->sDoAction(Action(actionName, actionType));
			}
		}

	}
}