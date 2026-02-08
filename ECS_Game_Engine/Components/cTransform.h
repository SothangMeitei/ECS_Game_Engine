#pragma once
#include"vector.h"

struct cTransform {
	vec2 position{ 0 , 0 };
	double scale{ 1.0 };
	double angle{ 1.0 };

	cTransform() {}
	cTransform(const vec2& position,
		const double scale,
		const double angle) :
		position{position}, scale{ scale }, angle{ angle }
	{}
};