#ifndef MODEL_H
#define MODEL_H

#include <GL/gl.h>
#include <vector>

#include "Maths.h"

using namespace std;

struct Model {
	GLuint texture;
	vector<vec3> vert;
	vector<vec2> vertTex;
	vector<vec3> vertNorm;
	//connection values
	vector<unsigned int> vIndices, vtIndices, vnIndices;
	vec3 pos; //moves along x and z
	Model ();
	Model ( const char * objFile, const char * texFile);
	void draw(int x, int z, float y=0, float yaw = 0);
	void drawTitleModel(float x, float y, float z, float yaw);
	bool GenerateModel( const char * objFile );
	bool GenerateTexture( const char * texFile );
private:
	//bool GenerateModel( const char * objFile );
	//bool GenerateTexture( const char * texFile );
};


#endif
