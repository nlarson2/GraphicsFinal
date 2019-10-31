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
	static void Normalize( vec3& vec);
	static float Distance(vec3& p1, vec3& p2);
	static void Slope(vec3& root, vec3& point, vec3& slope);
};


#endif
