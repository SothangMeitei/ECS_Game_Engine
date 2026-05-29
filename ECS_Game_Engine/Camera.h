#pragma once

#include<SDL3/SDL.h>
#include<vector>
#include"Components/vector.h"
#include<memory>
#include"ECS/Entity.h"
#include<functional>
#include<array>

//Will be included once in the scene that will require a camera
//and this camera will then make a certain part of the world be displayed to the current window

enum cameraSmoothingEffect {
	FOLLOW_ENTITY,
	FOLLOW_ENTITY_LAG,
	FOLLOW_ENTITY_LAG_MIRROR
};

class Camera
{
private:
	vec2 m_position;
	int w;
	int h;
	std::shared_ptr<Entity> m_cameraTarget;
	//this camera effect will calculate the new postion of the camera depending on the effect selected
	std::unordered_map<cameraSmoothingEffect, std::function<void()>> cameraEffectMap;
public:
	Camera() = default;

	Camera(const vec2& position, int w, int h , const std::shared_ptr<Entity>& entity)
		: m_position{ position }, w{ w }, h{ h }, m_cameraTarget{ entity } {
		//since this target is a shared pointer it will be pointing to the entity always
		//and will have the updated position always 

		cameraEffectMap[FOLLOW_ENTITY] = [this]() {
			m_position = m_cameraTarget->m_Transform->position;
			};
		cameraEffectMap[FOLLOW_ENTITY_LAG] = [this]() {
				//how to make this lag is quite difficult
			//maybe we record the positions into some form of buffer and then use it later to update this cam position 
			//with that stored value of the past positions 
			};
		cameraEffectMap[FOLLOW_ENTITY_LAG_MIRROR] = [this]() {};
	}
	
	void setPosition(const vec2& newPosition) {
		m_position = newPosition;
	}

	void setTarget(const std::shared_ptr<Entity>& newCameraTarget) {
		m_cameraTarget = newCameraTarget;
	}

	std::shared_ptr<Entity> getTarget() {
		return m_cameraTarget;
	}
	vec2 getPosition() {
		return m_position;
	}
	std::array<int, 2> getDimensions() {
		return { w , h };
	}
};

