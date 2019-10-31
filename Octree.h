#ifndef OCTREE_H
#define OCTREE_H

#include "Maths.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <GL/glu.h>

using namespace std;

static vec3 split[8] = {
    
    vec3(-1.0f, 1.0f, 1.0f), //LUF
    vec3(-1.0f, 1.0f,-1.0f), //LUB
    vec3(-1.0f,-1.0f, 1.0f), //LDF
    vec3(-1.0f,-1.0f,-1.0f), //LDB
    vec3( 1.0f, 1.0f, 1.0f), //RUF
    vec3( 1.0f, 1.0f,-1.0f), //RUB
    vec3( 1.0f,-1.0f, 1.0f), //RDF
    vec3( 1.0f,-1.0f,-1.0f)  //RDB

};

struct Cube{
    vec3 pos;
    vec3 color;
    float size;
};


struct OctreeNode{
    OctreeNode * node;
    vec3 color;
    int level;
    OctreeNode(){}
    OctreeNode(int _level)
    {
        level = _level;
    }
};

class Octree{
    private:
        int maxDepth;
        int depth;
        OctreeNode root;
        vec3 pos;
        vec3 origin;
        float size;
        vector<Cube> cubes;
    public:
        Octree(int _maxDepth, int _size)
        {
            
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = -20.0f;
            srand(time(nullptr));
            size = _size;
            root.level = 0;
            maxDepth = _maxDepth;
            origin = vec3(0.0f, 0.0f, 0.0f);
            addNodes(root, 0, origin, size);
        }
        void addNodes(OctreeNode & node, int _depth, vec3 _pos, float _size)
        {
            if(_depth == maxDepth) {
                node.level = _depth;
                node.color.y = rand()%256;
                node.color.x = rand()%256;
                node.color.z = rand()%256;
                return;
            } else {
                node.level = _depth;
                node.node = new OctreeNode[8];
                for(int i = 0; i < 8; i++) {
                    vec3 newPos;
                    newPos.x = _pos.x + (_size * split[i].x) / 2;
                    newPos.y = _pos.y + (_size * split[i].y) / 2;
                    newPos.z = _pos.z + (_size * split[i].z) / 2;

                    //get slope move the point
                    vec3 slope;
                    vec3 ray = this->origin;
                    
                    //ray.x += size/2;
                    vec3::Slope(ray, newPos, slope);
                    vec3::Normalize(slope);
                     if(newPos.y > 0) {
                    printf("Origin: X: %f, Y:%f, Z:%f\n", ray.x, ray.y, ray.z);
                    printf("NewPos: X: %f, Y:%f, Z:%f\n", newPos.x, newPos.y, newPos.z);
                    printf("Slope: X: %f, Y:%f, Z:%f\n\n", slope.x, slope.y, slope.z);
                    }
                    bool hit = false;
                    for (float i = 0.1f; i < size/2; i += 0.1f) {
                        ray = slope * 0.1f;
                        if(ray.x < newPos.x + _size/2 && ray.x > newPos.x - _size/2 && 
                            ray.y < newPos.y + _size/2 && ray.y > newPos.y - _size/2 && 
                            ray.z < newPos.z + _size/2 && ray.z > newPos.z - _size/2) {
                                hit = true;
                                break;

                            }
                    }
                    if(hit)
                        addNodes(node.node[i], _depth + 1, newPos, _size/2.0f);
                    else
                        continue;
                }
                
            }
        }
        void printOctree()
        {
            printOctree(root.node);
            printf("\n");
        }
        void printOctree(OctreeNode * node) 
        {

            if (node == nullptr)
                return;
            if(node == &root) {
                printOctree(node->node);
                return;
            } else {
                for(int i = 0; i < 8; i++) {
                    if(node->node == nullptr)
                        continue;
                    printOctree(&(node->node[i]));
                }
            }
            cout << node->level;
        }
        void genOctreeModel(int depth = 0)
        {
            
            genOctreeModel(root.node, maxDepth, vec3(0.0f, 0.0f, 0.0f), size);
            
        }
        void genOctreeModel(OctreeNode * node, int depth, vec3 _pos, float _size)
        {
            
            if (node == nullptr)
                return;
            if( depth == 1) {//draw?
                
                Cube cube;
                cube.color = node->color;
                cube.pos = _pos;
                cube.size = _size;
                cubes.push_back(cube);
                return;
            }
            for(int i = 0; i < 8; i++) {
                if(node->node == nullptr)
                    continue;
                /*
                vec3 newPos(split[i].x, split[i].y, split[i].z);
                newPos = newPos * _size / 2;
                newPos += _pos;
                */
                vec3 newPos;
                newPos.x = _pos.x + (_size * split[i].x) / 2;
                newPos.y = _pos.y + (_size * split[i].y) / 2;
                newPos.z = _pos.z + (_size * split[i].z) / 2;
                
                genOctreeModel(&(node->node[i]), depth - 1, newPos, _size/2.0f);
            }
        }
        void draw() {
            static float rot = 0.0f;
            rot += 0.3f;
            glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            glRotatef(rot, 0.5f, 0.8f, 0.0f);
            for(unsigned int i = 0; i < cubes.size(); i++) {
                glBegin(GL_QUADS);
                    glColor3f(cubes[i].color.x/255, cubes[i].color.y/255, cubes[i].color.z/255);
                    // Top 
                    glVertex3f( cubes[i].size + cubes[i].pos.x,  cubes[i].size + cubes[i].pos.y, -cubes[i].size + cubes[i].pos.z);  // Top-right of top face
                    glVertex3f(-cubes[i].size + cubes[i].pos.x,  cubes[i].size + cubes[i].pos.y, -cubes[i].size + cubes[i].pos.z);  // Top-left of top face
                    glVertex3f(-cubes[i].size + cubes[i].pos.x,  cubes[i].size + cubes[i].pos.y,  cubes[i].size + cubes[i].pos.z);  // Bottom-left of top face
                    glVertex3f( cubes[i].size + cubes[i].pos.x,  cubes[i].size + cubes[i].pos.y,  cubes[i].size + cubes[i].pos.z);  // Bottom-right of top face
            
                    // Bottom face
                    glVertex3f( cubes[i].size + cubes[i].pos.x, -cubes[i].size + cubes[i].pos.y, -cubes[i].size + cubes[i].pos.z); // Top-right of bottom face
                    glVertex3f(-cubes[i].size + cubes[i].pos.x, -cubes[i].size + cubes[i].pos.y, -cubes[i].size + cubes[i].pos.z); // Top-left of bottom face
                    glVertex3f(-cubes[i].size + cubes[i].pos.x, -cubes[i].size + cubes[i].pos.y,  cubes[i].size + cubes[i].pos.z); // Bottom-left of bottom face
                    glVertex3f( cubes[i].size + cubes[i].pos.x, -cubes[i].size + cubes[i].pos.y,  cubes[i].size + cubes[i].pos.z); // Bottom-right of bottom face
                    
                    // Front face
                    glVertex3f( cubes[i].size + cubes[i].pos.x,  cubes[i].size + cubes[i].pos.y,  cubes[i].size + cubes[i].pos.z);  // Top-Right of front face
                    glVertex3f(-cubes[i].size + cubes[i].pos.x,  cubes[i].size + cubes[i].pos.y,  cubes[i].size + cubes[i].pos.z);  // Top-left of front face
                    glVertex3f(-cubes[i].size + cubes[i].pos.x, -cubes[i].size + cubes[i].pos.y,  cubes[i].size + cubes[i].pos.z);  // Bottom-left of front face
                    glVertex3f( cubes[i].size + cubes[i].pos.x, -cubes[i].size + cubes[i].pos.y,  cubes[i].size + cubes[i].pos.z);  // Bottom-right of front face
                
                    // Back face
                    glVertex3f( cubes[i].size + cubes[i].pos.x, -cubes[i].size + cubes[i].pos.y, -cubes[i].size + cubes[i].pos.z); // Bottom-Left of back face
                    glVertex3f(-cubes[i].size + cubes[i].pos.x, -cubes[i].size + cubes[i].pos.y, -cubes[i].size + cubes[i].pos.z); // Bottom-Right of back face
                    glVertex3f(-cubes[i].size + cubes[i].pos.x,  cubes[i].size + cubes[i].pos.y, -cubes[i].size + cubes[i].pos.z); // Top-Right of back face
                    glVertex3f( cubes[i].size + cubes[i].pos.x,  cubes[i].size + cubes[i].pos.y, -cubes[i].size + cubes[i].pos.z); // Top-Left of back face
                
                
                    // Left face
                    glVertex3f(-cubes[i].size + cubes[i].pos.x,  cubes[i].size + cubes[i].pos.y,  cubes[i].size + cubes[i].pos.z);  // Top-Right of left face
                    glVertex3f(-cubes[i].size + cubes[i].pos.x,  cubes[i].size + cubes[i].pos.y, -cubes[i].size + cubes[i].pos.z);  // Top-Left of left face
                    glVertex3f(-cubes[i].size + cubes[i].pos.x, -cubes[i].size + cubes[i].pos.y, -cubes[i].size + cubes[i].pos.z);  // Bottom-Left of left face
                    glVertex3f(-cubes[i].size + cubes[i].pos.x, -cubes[i].size + cubes[i].pos.y,  cubes[i].size + cubes[i].pos.z);  // Bottom-Right of left face
                    // Right face
                    glVertex3f( cubes[i].size + cubes[i].pos.x,  cubes[i].size + cubes[i].pos.y,  cubes[i].size + cubes[i].pos.z);  // Top-Right of left face
                    glVertex3f( cubes[i].size + cubes[i].pos.x,  cubes[i].size + cubes[i].pos.y, -cubes[i].size + cubes[i].pos.z);  // Top-Left of left face
                    glVertex3f( cubes[i].size + cubes[i].pos.x, -cubes[i].size + cubes[i].pos.y, -cubes[i].size + cubes[i].pos.z);  // Bottom-Left of left face
                    glVertex3f( cubes[i].size + cubes[i].pos.x, -cubes[i].size + cubes[i].pos.y,  cubes[i].size + cubes[i].pos.z);  // Bottom-Right of left face
                glEnd();
            }
            glPopMatrix();
        }
        //traverse(depth)
        //
};



#endif
