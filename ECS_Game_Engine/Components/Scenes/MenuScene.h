#pragma once
#include"../../AbstractScene.h"


//the menu scene has a task bar and then within each task bar there is a new array of options to choose from 
//this can be done using either the mouse or the keyboard buttons

//all the allowed keyboard buttons are up(w) , down(s) , left(a) , right(d) 
//depending on the task bar or the options within the task bar where the control is 
//they have different actions
//eg in the task bar the left will choose the immediate left of the current option where the control is
//		and then this will be different for the part where the option of changing the say sound left will decrece the sound

//the mouse can click any anywhere on the screen and the control will latch on to that part of the screen

class MenuScene : public AbstractScene {
private:

public:
	MenuScene(const std::string_view);

	void play() override;
	void sDoAction(const Action&) override;
	void updateInternals() override;
	void render() override;
};