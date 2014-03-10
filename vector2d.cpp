// vector2d.cpp: David R. Morrison, March 2014
// Implementation for the 2d vector

#include "vector2d.h"

#include <cmath>

Vector2D& Vector2D::operator+=(const Vector2D& rhs)
	{ x += rhs.x; y += rhs.y; return *this; }
Vector2D& Vector2D::operator-=(const Vector2D& rhs)
	{ x -= rhs.x; y -= rhs.y; return *this; }
Vector2D& Vector2D::operator*=(double scalar)
	{ x *= scalar; y *= scalar; return *this; }
Vector2D& Vector2D::operator/=(double scalar)
	{ x /= scalar; y /= scalar; return *this; }

double length(const Vector2D& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

void normalize(Vector2D& v)
{
	v /= length(v);
}

const Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
	{ return Vector2D(v1.x + v2.x, v1.y + v2.y); }
const Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
	{ return (v1 + (-1 * v2)); }
const Vector2D operator*(double scalar, const Vector2D& v)
	{ return Vector2D(scalar * v.x, scalar * v.y); }
const Vector2D operator*(const Vector2D& v, double scalar)
	{ return operator*(scalar, v); }
const Vector2D operator/(const Vector2D& v, double scalar)
	{ return Vector2D(v.x / scalar, v.y / scalar); }

