#pragma once
#include"../../AbstractScene.h"


/*
	Very simple menu scene just for the purpose of having one

	there are to be boxes that will lead to other scenes (other menu scenes or other gamePlay scene)
	the boxes are interactable entities that have the component of menu box
	this menu box component has some other pointer to some other scene maybe or some form of slider or some form of box boundary outline margin
	that indicates that this box has been selected or something like that 

	for the first menu scene this is just some simple stuffs like that of boxes that are arranged in a list and navigated using up down button
	this boxes will be the levels and have level indicators in the form of a text on the box and 
	selecting this box will make one go to the gameplay scene of that level or make the game engine display this gameplay scene

	then this gameplay scene becomes the scene that the engine is playing

	and when the player pushes some button that indicates exit then it goes back to the main menu scene

	mechanics of the working

	the boxes are arranged in terms of simple predefined rectangles with coordinates that are predefined
	then there is one selector that the player is controlling this will move in terms of this dimension then select the boxes
	the position of this selector is recorded in the state of the scene so that when the player comes back to this main menu it is in that place
	then if the selector goes down below the most visible part of the window then the levels that are under comes up
*/

class MenuScene : public AbstractScene {
private:
	int			numberOfLevels;
	std::shared_ptr<Entity> m_selectorEntity;
	int m_selectedIndex{ 0 };

	float m_startX{ 200.0f };
	float m_startY{ 100.0f };
	float m_padding{ 20.0f };
	int m_rectHeight{ 0 };

public:
	MenuScene(const std::string_view);

	void play(float) override;					//this just starts the showing of this scene that is start the action update and render
	void sDoAction(const Action&) override;	//each text box will contain the name of the scene that it will go to and not the actual scene pointer the game engine will handle the chainging of the current scene
	void updateInternals(float) override;		//dynamically add new scenes update the position of the selector rectangle
	void render() override;
};