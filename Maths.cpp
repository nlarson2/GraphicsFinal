#ifndef MATHS_CPP
#define MATHS_CPP

#include "Maths.h"


#include <cmath>


vec2::vec2() 
{
	x = y = 0;
}
vec2::vec2(float _x,float _y)
{
	x = _x;
	y = _y;
}

vec2 vec2::operator = (const vec2& right)
{
	x = right.x;
	y = right.y;
	return *this;
}
vec2 vec2::operator +(const vec2& right)
{
	vec2 ret;
	ret.x = x + right.x;
	ret.y = y + right.y;
	return ret;

}
vec2 vec2::operator +=(const vec2& right)
{
	return *this + right;

}
//---------------------------------------
//vec3-----------------------------------
vec3::vec3()
{
	x = y = z = 0;
}

vec3::vec3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void vec3::operator()(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}
vec3 vec3::operator = (const vec3& right)
{	
	x = right.x;
	y = right.y;
	z = right.z;
	return *this;
}
vec3 vec3::operator +(const vec3& right)
{
	vec3 ret;
	ret.x = x + right.x;
	ret.y = y + right.y;
	ret.z = z + right.z;
	return ret;
}
vec3 vec3::operator +=(const vec3& right)
{
	this->x += right.x;
	this->y += right.y;
	this->z += right.z;
	return *this;
}
vec3 vec3::operator -(const vec3& right)
{
	vec3 ret;
	ret.x = x - right.x;
	ret.y = y - right.y;
	ret.z = z - right.z;
	return ret;
}
vec3 vec3::operator -=(const vec3& right)
{
	this->x -= right.x;
	this->y -= right.y;
	this->z -= right.z;
	return *this;
}
vec3 vec3::operator*(float scale)
{
	return vec3(x*scale, y*scale, z*scale);
}
vec3 vec3::operator/(float scale)
{
	return vec3(x/scale, y/scale, z/scale);
}

vec3 vec3::crossProd(const vec3& left, const vec3& right)
{
	vec3 ret;
	ret.x = left.y*right.z - left.z*right.y;
	ret.y = left.z*right.x - left.x*right.z;
	ret.z = left.x*right.y - left.y*right.x;
	return ret;
}
float vec3::Magnitude(vec3& vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}
void vec3::Normalize(vec3& vec)
{
	vec = vec/Magnitude(vec);
}
float vec3::Distance(vec3& p1, vec3& p2)
{
	float dist = (p1.x - p2.x)*(p1.x - p2.x);
	dist += (p1.y - p2.y)*(p1.y - p2.y);
	dist += (p1.z - p2.z)*(p1.z - p2.z);
	return sqrt(dist);
}

void vec3::Slope(vec3& root, vec3& point, vec3& slope)
{
	slope.x = point.x - root.x;
	slope.y = point.y - root.y;
	slope.z = point.z - root.z;
}

#endif
