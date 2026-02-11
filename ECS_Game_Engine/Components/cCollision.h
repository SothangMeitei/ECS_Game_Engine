#pragma once
struct cCollision {
	//this is just the component so this will just store only 
	//the part of the hit box that will represent the boundary of 
	//the collider physics of the object that has this as a component

	//for now we make this a rectangle of 
	int w{ 0 };
	int h{ 0 };
	
	cCollision() {}
	cCollision(int w, int h) : w(w), h(h) {}
};