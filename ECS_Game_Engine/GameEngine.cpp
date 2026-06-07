#pragma once
#include"GameEngine.h"
#include"AbstractScene.h"
#include"Components/Action.h"

GameEngine::GameEngine() {
	m_isRunning = true;
	m_paused = false;

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		SDL_Log("Failure to initialize SDL subsystems: %s", SDL_GetError());
		m_isRunning = false;
		return;
	}

	if (!SDL_CreateWindowAndRenderer("Window", 900, 400, SDL_WINDOW_RESIZABLE, &m_gameWindow, &m_gameRenderer)) {
		SDL_Log("Failure to create window and renderer: %s", SDL_GetError());
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
		auto currentScene = m_mapSceneNameToSceneObject[getCurrentSceneName()];
		if (currentScene) {
			currentScene->play(1);
		}
	}
}
void GameEngine::quit(){m_isRunning = false;}

void GameEngine::addScene(const std::string& newSceneName , std::shared_ptr<AbstractScene> scene){
	m_mapSceneNameToSceneObject[newSceneName] = scene;
}
const std::string& GameEngine::getCurrentSceneName() const {
	return m_currentSceneName;
}
bool GameEngine::hasScene(const std::string& sceneName) const {
	return m_mapSceneNameToSceneObject.find(sceneName) != m_mapSceneNameToSceneObject.end();
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
		if (event.type == SDL_EVENT_QUIT) {
			quit();
		}
		if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
			std::string actionType{ (event.type == SDL_EVENT_KEY_DOWN) ? "START" : "END" };

			auto& currentScene = m_mapSceneNameToSceneObject[getCurrentSceneName()];
			auto& currentSceneInputMap{ currentScene->getActionMap() };
			int currentKey = event.key.scancode;

			// if this current scene has a mapping for this current key press
			//create the action for this and then sent to the current scene
			if (currentSceneInputMap.find(currentKey) != currentSceneInputMap.end()) {
				const std::string& actionName = currentSceneInputMap.at(currentKey);
				currentScene->sDoAction(Action(actionName, actionType));
			}
		}

	}
}