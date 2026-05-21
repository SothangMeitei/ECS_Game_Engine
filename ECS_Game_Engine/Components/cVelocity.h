#pragma once
#include"vector.h"

//velocity is a 2d vector in the 2 dimensions
struct cVelocity {
	vec2 velocity{0.0 , 0.0};
	double speed{ 0.0 };
	cVelocity() {}
	cVelocity(vec2 v , double speed) : velocity{ v } , speed(speed) {}
	cVelocity(vec2 v) : velocity{ v }{}
};