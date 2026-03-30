#pragma once
struct cShape
{
	//just the hit box of this element
	//rectangle 
	float w;
	float h;

	int r, g, b;
	int vertices;

	cShape(float width, float height, int red, int green, int blue , int v)
		: w(width), h(height), r(red), g(green), b(blue) , vertices(v){
	}
};

