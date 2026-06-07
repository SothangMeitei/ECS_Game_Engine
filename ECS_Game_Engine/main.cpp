//#include"game.h"
//#include"GameEngine.h"
//
//int main(int argc, char* argv[]) {
//	//game g("config.txt");
//	//g.start();
//
//	GameEngine gameEngine1;
//	gameEngine1.start();
//	
//	return 0;
//}
#include "game.h"
#include "GameEngine.h"
#include "Components/Scenes/gamePlayScene.h"
#include "Components/Scenes/MenuScene.h"

int main(int argc, char* argv[]) {
    GameEngine gameEngine;

    auto playScene = std::make_shared<gamePlayScene>(&gameEngine, "config.txt");
    auto menuScene = std::make_shared<MenuScene>(&gameEngine, "./Components/Scenes/menuSceneConfiguration.txt");

    gameEngine.addScene("Play", playScene);
    gameEngine.addScene("Menu", menuScene);

    gameEngine.changeScene("Menu");

    gameEngine.start();

    return 0;
}