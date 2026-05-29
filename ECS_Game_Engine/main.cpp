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

int main(int argc, char* argv[]) {
    GameEngine gameEngine1;

    // Pass the memory address of the engine into the scene constructor!
    std::shared_ptr<gamePlayScene> myScene = std::make_shared<gamePlayScene>(&gameEngine1, "config.txt");

    gameEngine1.addScene("Play", myScene);
    gameEngine1.changeScene("Play");
    gameEngine1.start();

    return 0;
}