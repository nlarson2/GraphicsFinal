#ifndef MATHS_CPP
#define MATHS_CPP

#include "Maths.h"


#include <cmath>


template <class T>
T power (T val, int exp)
{
	if (exp == 1) {
		return val;
	}
	return val * power(val, exp-1);
}



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
vec3 vec3::Normalize(const vec3& vec)
{
	vec3 ret = vec;
	return (ret/Magnitude(ret));
}


float distance3D(float x1, float y1, float z1,  float x2, float y2, float z2)
{
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

float distance3D(vec3 v1, vec3 v2)
{
	return sqrt((v2.x-v1.x)*(v2.x-v1.x) + (v2.y-v1.y)*(v2.y-v1.y) + (v2.z-v1.z)*(v2.z-v1.z));
}

bool rayCubeCollision(vec3 ray, vec3 pos, float size)
{
	if(ray.x <= pos.x + size && ray.x >= pos.x - size &&
		ray.y <= pos.y + size && ray.y >= pos.y - size && 
		ray.z <= pos.z + size && ray.z >= pos.z - size)
		return true;
	return false;
}


#endif
