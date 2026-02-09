#pragma once
struct cShape
{
	//just the hit box of this element
	//rectangle 
	float w;
	float h;

	int r, g, b;

	cShape(float width, float height, int red, int green, int blue)
		: w(width), h(height), r(red), g(green), b(blue) {
	}
};

