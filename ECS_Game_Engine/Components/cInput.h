#pragma once

struct cInput {
	//in this the input is to be latched into a boolean
	//when there is an input the boolean that is latched
	//and then state of this component is set

	bool up{false};
	bool down{ false };
	bool left{ false };
	bool right{ false };

	cInput() {}
	cInput(bool up , bool down , bool left , bool right)
		: up(up) , down(down) , right(right) , left(left){}
};