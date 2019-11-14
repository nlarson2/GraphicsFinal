#ifndef MATHS_H
#define MATHS_H


struct vec2 {
	float x, y;
	vec2();
	vec2(float x, float z);
	vec2 operator = (const vec2& right);
	vec2 operator +(const vec2& right);
	vec2 operator +=(const vec2& right);
};

struct vec3 {
	float x, y, z;
	vec3();
	vec3(float x, float y, float z);
	void operator()(float _x, float _y, float _z);
	vec3 operator = (const vec3& right);
	vec3 operator +(const vec3& right);
	vec3 operator +=(const vec3& right);
	vec3 operator -(const vec3& right);
	vec3 operator -=(const vec3& right);
	vec3 operator*(float scale);
	vec3 operator/(float scale);

	static vec3 crossProd(const vec3& left, const vec3& right);
	static float Magnitude(vec3& vec);
	static vec3 Normalize(const vec3& vec);
};


float distance3D(float x1, float y1, float z1, float x2, float y2, float z2);
float distance3D(vec3 v1, vec3 v2);
bool rayCubeCollision(vec3 ray, vec3 pos, float size);

#endif
