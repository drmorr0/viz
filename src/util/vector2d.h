#ifndef VECTOR2D_H
#define VECTOR2D_H
/* vector2d.h: David R. Morrison, March 2014
 * 
 * Defines a 2d vector struct with standard vector operations
 *
 */

struct Vector2D
{
	Vector2D() : x(0), y(0) { } 
	Vector2D(double _x, double _y) : x(_x), y(_y) { }
	double x, y;
	Vector2D& operator+=(const Vector2D& rhs);
	Vector2D& operator-=(const Vector2D& rhs);
	Vector2D& operator*=(double scalar);
	Vector2D& operator/=(double scalar);
};

double length (const Vector2D& v);
void normalize(Vector2D& v);

const Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
const Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
const Vector2D operator*(double scalar, const Vector2D& v);
const Vector2D operator*(const Vector2D& v, double scalar);
const Vector2D operator/(double scalar, const Vector2D& v);
const Vector2D operator/(const Vector2D& v, double scalar);

#endif // VECTOR2D_H
