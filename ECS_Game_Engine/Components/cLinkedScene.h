#pragma once
#include<string>
struct cLinkedScene
{
	std::string name_of_scene_link;
	cLinkedScene() = default;
	cLinkedScene(const std::string& inputName) : name_of_scene_link{ inputName } {}
};

