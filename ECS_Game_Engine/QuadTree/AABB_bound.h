#pragma once
struct AABB_Bounds{
	float x, y, w, h;

	bool intersect(const AABB_Bounds& other) const {
		return (x < other.x + other.w
			&& x + w > other.x
			&& y < other.y + other.h
			&& y + h > other.y);
	}
};