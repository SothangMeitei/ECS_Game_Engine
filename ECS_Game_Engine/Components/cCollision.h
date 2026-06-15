#pragma once
struct cCollision {
	//this is just the component so this will just store only 
	//the part of the hit box that will represent the boundary of 
	//the collider physics of the object that has this as a component

	//for now we make this a rectangle of 
	float w;
	float h;
	float offsetX;
	float offsetY;
	
	cCollision(int w, int h , float x , float y) : w(w), h(h) ,offsetX(x) , offsetY(y) {}
};