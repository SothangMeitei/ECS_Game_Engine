#pragma once

class vec2 {
public:
	double x, y;
	vec2();
	vec2(double, double);

	vec2 operator + (const vec2& rhs) const;
	vec2 operator - (const vec2& rhs) const;
	vec2 operator * (const double scale)const;

	void operator += (const vec2& rhs);
	void operator -= (const vec2& rhs);
	void operator *= (const vec2& rhs);

	bool operator == (const vec2& rhs);
	bool operator != (const vec2& rhs);

	void set_x(double);
	void set_y(double);

	double get_x();
	double get_y();

	void normalize();
	double length();
	double dist(const vec2& rhs) const;
	double dot(const vec2& rhs) const;
};