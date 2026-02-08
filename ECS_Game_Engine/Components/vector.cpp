#include"vector.h"
#include<cmath>

//-------------------------------------------------------------------

vec2::vec2() {
	this->x = 0;
	this->y = 0;
}
vec2::vec2(double ix , double iy) {
	this->x = ix;
	this->y = iy;
}

//-------------------------------------------------------------------

vec2& vec2::operator + (const vec2& rhs){
	this->x += rhs.x;
	this->y += rhs.y;

	return *this;
}
vec2& vec2::operator - (const vec2& rhs){
	this->x -= rhs.x;
	this->y -= rhs.y;

	return *this;
}
vec2& vec2::operator * (const vec2& rhs){
	this->x *= rhs.x;
	this->y *= rhs.y;

	return *this;
}

//-------------------------------------------------------------------

void vec2::operator += (const vec2& rhs){
	this->x += rhs.x;
	this->y += rhs.y;
}
void vec2::operator -= (const vec2& rhs){
	this->x -= rhs.x;
	this->y -= rhs.y;
}
void vec2::operator *= (const vec2& rhs){
	this->x *= rhs.x;
	this->y *= rhs.y;
}

//-------------------------------------------------------------------
bool vec2::operator == (const vec2& rhs) {
	return (this->x == rhs.x && this->y == rhs.y);
}

bool vec2::operator != (const vec2& rhs) {
	return (this->x != rhs.x || this->y != rhs.y);
}

//-------------------------------------------------------------------

double vec2::length() {
	return std::sqrt((x * x) + (y * y));
}

void vec2::normalize() {
	double length{ this->length() };
	this->x = this->x / length;
	this->y = this->y / length;
}

//-------------------------------------------------------------------

void vec2::set_x(double x){
	this->x = x;
}
void vec2::set_y(double y) { this->y = y; }

double vec2::get_x() { return this->x; }
double vec2::get_y() { return this->y; }