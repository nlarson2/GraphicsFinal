#ifndef MODEL_CPP
#define MODEL_CPP

#include "Model.h"

#include <stdio.h>
#include <cstring>
#include <string>
#include <cmath>

#include "Image.h"

extern void GenerateGLTexture(GLuint & texture, const char * dataSrc, bool inverted);



Model::Model() {
	const char * objFile = "models/characters/Tank.obj";
	const char * texFile = "models/characters/ModelTexturePlayer.png";
	if (!GenerateModel(objFile)) {
		printf("Failed to generate model\n");
	}
	if (!GenerateTexture(texFile)) {
		printf("Failed to load texture\n");
	}
}

Model::Model( const char * objFile, const char * texFile )
{
	if (!GenerateModel(objFile)) {
		printf("Failed to generate model\n");
	}
	if (!GenerateTexture(texFile)) {
		printf("Failed to load texture\n");
	}
}

void Model::draw(int x, int z, float y, float yaw) 
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	
	float posx = x;
	float posz = z;
	float posy = y; 
	
	if (z%2 == 0)
		posx -= 1.0f;
	glTranslatef(posx, posy, posz);
	
	if ( yaw > 0 ) {
		glRotatef(yaw, 0, 1.0f, 0);
	}	
	glBegin(GL_TRIANGLES);

	for ( unsigned int i = 0 ; i < vIndices.size() ; i+=3 ) {
		glTexCoord2f(vertTex[vtIndices.at(i)-1].x,
				vertTex[vtIndices.at(i)-1].y);
		glVertex3f(vert[vIndices.at(i)-1].x ,
				vert[vIndices.at(i)-1].y,
				vert[vIndices.at(i)-1].z);
		glTexCoord2f(vertTex[vtIndices.at(i+1)-1].x,
				vertTex[vtIndices.at(i+1)-1].y);
		glVertex3f(vert[vIndices.at(i+1)-1].x,
				vert[vIndices.at(i+1)-1].y,
				vert[vIndices.at(i+1)-1].z);
		glTexCoord2f(vertTex[vtIndices.at(i+2)-1].x,
				vertTex[vtIndices.at(i+2)-1].y);
		glVertex3f(vert[vIndices.at(i+2)-1].x,
				vert[vIndices.at(i+2)-1].y,
				vert[vIndices.at(i+2)-1].z);
	}
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Model::drawTitleModel(float x, float y, float z, float yaw) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(x, y, z);
	glRotatef(yaw, 0, 1.0f, 0);
		
	glBegin(GL_TRIANGLES);

	for ( unsigned int i = 0 ; i < vIndices.size() ; i+=3 ) {
		glTexCoord2f(vertTex[vtIndices.at(i)-1].x,
				vertTex[vtIndices.at(i)-1].y);
		glVertex3f(vert[vIndices.at(i)-1].x ,
				vert[vIndices.at(i)-1].y,
				vert[vIndices.at(i)-1].z);
		glTexCoord2f(vertTex[vtIndices.at(i+1)-1].x,
				vertTex[vtIndices.at(i+1)-1].y);
		glVertex3f(vert[vIndices.at(i+1)-1].x,
				vert[vIndices.at(i+1)-1].y,
				vert[vIndices.at(i+1)-1].z);
		glTexCoord2f(vertTex[vtIndices.at(i+2)-1].x,
				vertTex[vtIndices.at(i+2)-1].y);
		glVertex3f(vert[vIndices.at(i+2)-1].x,
				vert[vIndices.at(i+2)-1].y,
				vert[vIndices.at(i+2)-1].z);
	}
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}


bool Model::GenerateModel( const char * objFile) {
	FILE * inFile= fopen(objFile,"r");
	if (!inFile){
		return 0;
	}
	while (1){

		char line[128];
		if (fscanf(inFile,"%s",line)==EOF) {
			break;
		}

		//v=vertices vt=textureCoords vn=normals f=faces(holds multiple datas
		if (strcmp(line,"v")==0){
			vec3 vertex;
			fscanf(inFile,"%f %f %f\n",
					&vertex.x,&vertex.y,&vertex.z);
			vert.push_back(vertex);
		} else if (strcmp(line,"vt")==0){
			vec2 texture;
			fscanf(inFile,"%f %f\n",
					&texture.x,&texture.y);
			vertTex.push_back(texture);
		} else if (strcmp(line,"vn")==0){
			vec3 norm;
			fscanf(inFile,"%f %f %f\n", &norm.x, &norm.y, &norm.z);
			vertNorm.push_back(norm);
		} else if (strcmp(line,"f")==0){
			string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3],
			textureIndex[3],normalIndex[3];
			int matches = fscanf(inFile,
					"%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &textureIndex[0], &normalIndex[0],
					&vertexIndex[1], &textureIndex[1], &normalIndex[1],
					&vertexIndex[2], &textureIndex[2], &normalIndex[2] );
			if (matches != 9){
#ifdef UNIT_TEST
				printf("%d Failed to read\n",matches);
#endif
				return false;
			}
			//save all the data where it needs to be stored
			vIndices.push_back(vertexIndex[0]);
			vIndices.push_back(vertexIndex[1]);
			vIndices.push_back(vertexIndex[2]);
			vtIndices.push_back(textureIndex[0]);
			vtIndices.push_back(textureIndex[1]);
			vtIndices.push_back(textureIndex[2]);
			vnIndices.push_back(normalIndex[0]);
			vnIndices.push_back(normalIndex[1]);
			vnIndices.push_back(normalIndex[2]);
		}
	}
	fclose(inFile);
	return true;
}

bool Model::GenerateTexture ( const char * texFile ) {
	GenerateGLTexture(texture, texFile, true);
	return glIsTexture(texture);
}

#endif
