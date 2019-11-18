#ifndef OCTREE_H
#define OCTREE_H

#include <GL/glu.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <fstream>


#include "Maths.h"
#include "Model.h"

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
    float size;
    vec3 pos;
    bool isLeaf;
    OctreeNode(){
        isLeaf = false;
    }
    OctreeNode(int _level)
    {
        level = _level;
    }
};

class Octree{
    private:
        int maxDepth;
        int depth;
        OctreeNode * root;
        vec3 pos;
        float size;
        float radius;
        vector<Cube> cubes;
    public:
        Octree(int _maxDepth, int _size)
        {
            radius = _size/2;
            pos.x = -0.0f;
            pos.y = -0.0f;
            pos.z = -20.0f;
            srand(time(nullptr));
            size = _size;
            root = new OctreeNode();
            root->level = 0;
            maxDepth = _maxDepth;
            //addNodes(root);
            //addSphere(&root, size, vec3(0,0,0));
            for(int i = 1; i <= maxDepth; i++) {
               // addNodes(root, i);
               addNodesSphere(root, i, vec3(0,0,0), size/2.0f);
            }
            //printOctree();
            outputFile("test.txt");
        }

        void addNodes(OctreeNode * node, int level) {

          
            if (node->node == nullptr) {
                node->node = new OctreeNode[8];
                node->isLeaf = false;
                for(int i = 0; i < 8; i++) {
                    node->node[i].node = nullptr;
                    node->node[i].color = vec3(rand()%255, rand()%255, rand()%255);
                    node->node[i].level = level;
                    node->node[i].isLeaf = true;
                    //printf("OCTREE LEVEL: %d created\n", level);
                }
                return;
            }

              
            for(int i = 0; i < 8; i++) {
                node->node[i].isLeaf = false;
                addNodes(&node->node[i], level);
            }
        }
        void addNodesSphere(OctreeNode * node, int level, vec3 _pos, float _size) {

          
            if (node->node == nullptr) {
                node->node = new OctreeNode[8];
                node->isLeaf = false;
                for(int i = 0; i < 8; i++) {
                    node->node[i].node = nullptr;
                    node->node[i].color = vec3(rand()%255, rand()%255, rand()%255);
                    node->node[i].level = level;
                    node->node[i].isLeaf = true;
                    //printf("OCTREE LEVEL: %d created\n", level);
                }
                return;
            }

              
            for(int i = 0; i < 8; i++) {

                vec3 newPos;
                newPos.x = _pos.x + (_size * split[i].x) / 2;
                newPos.y = _pos.y + (_size * split[i].y) / 2;
                newPos.z = _pos.z + (_size * split[i].z) / 2;
                node->node[i].isLeaf = false;
                
                /*vec3 newPos = _pos;
                newPos += (split[i] * _size / 2);*/
                if(distance3DCube(0,0,0, newPos.x, newPos.y, newPos.z, _size/2.0f) < 1.8f) {
                    addNodesSphere(&node->node[i], level, newPos, _size/2.0f);    
                    //genOctreeModel(&(node->node[i]), depth, newPos, _size/2.0f);
                }
            }
        }
        void printOctree()
        {
            printOctree(root);//->node);
            printf("\n");
        }
        void printOctree(OctreeNode * node) 
        {

            if (node == nullptr)
                return;
            if(node == root) {
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
            if(node->isLeaf)
                cout << "!";
        }
        void genOctreeModel(int depth = 0)
        {
            
            genOctreeModel(root, 0, vec3(0.0f, 0.0f, 0.0f), size);
            
        }

        void drawTree() {
            static float rot = 0.0f;
            rot += 0.3f;
            glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            glRotatef(rot, 0.5f, 0.8f, 0.0f);
            drawTree(0, root);
            glPopMatrix();
        }
        void drawTree(int depth, OctreeNode * node)
        {
            if(node == nullptr) {
                return;
            }
            if(node->level == maxDepth) {
                //printf("node level\n");
                drawNode(node);
                return;
            }

            if(node->node != nullptr) {
                for(int i = 0; i < 8; i++) {
                        drawTree(depth+1, &node->node[i]);
                }
            }

        }


        void drawNode(OctreeNode * node) {
            
           // printf("DRAW\n");
            glPushMatrix();
            glTranslatef(node->pos.x, node->pos.y, node->pos.z);
             glBegin(GL_QUADS);
                        glColor3f(node->color.x/255, node->color.y/255, node->color.z/255);
                        // Top 
                        glVertex3f( node->size,  node->size, -node->size);  // Top-right of top face
                        glVertex3f(-node->size,  node->size, -node->size);  // Top-left of top face
                        glVertex3f(-node->size,  node->size,  node->size);  // Bottom-left of top face
                        glVertex3f( node->size,  node->size,  node->size);  // Bottom-right of top face
                
                        // Bottom face
                        glVertex3f( node->size, -node->size, -node->size); // Top-right of bottom face
                        glVertex3f(-node->size, -node->size, -node->size); // Top-left of bottom face
                        glVertex3f(-node->size, -node->size,  node->size); // Bottom-left of bottom face
                        glVertex3f( node->size, -node->size,  node->size); // Bottom-right of bottom face
                        
                        // Front face
                        glVertex3f( node->size,  node->size,  node->size);  // Top-Right of front face
                        glVertex3f(-node->size,  node->size,  node->size);  // Top-left of front face
                        glVertex3f(-node->size, -node->size,  node->size);  // Bottom-left of front face
                        glVertex3f( node->size, -node->size,  node->size);  // Bottom-right of front face
                    
                        // Back face
                        glVertex3f( node->size, -node->size, -node->size); // Bottom-Left of back face
                        glVertex3f(-node->size, -node->size, -node->size); // Bottom-Right of back face
                        glVertex3f(-node->size,  node->size, -node->size); // Top-Right of back face
                        glVertex3f( node->size,  node->size, -node->size); // Top-Left of back face
                    
                    
                        // Left face
                        glVertex3f(-node->size,  node->size,  node->size);  // Top-Right of left face
                        glVertex3f(-node->size,  node->size, -node->size);  // Top-Left of left face
                        glVertex3f(-node->size, -node->size, -node->size);  // Bottom-Left of left face
                        glVertex3f(-node->size, -node->size,  node->size);  // Bottom-Right of left face
                        // Right face
                        glVertex3f( node->size,  node->size,  node->size);  // Top-Right of left face
                        glVertex3f( node->size,  node->size, -node->size);  // Top-Left of left face
                        glVertex3f( node->size, -node->size, -node->size);  // Bottom-Left of left face
                        glVertex3f( node->size, -node->size,  node->size);  // Bottom-Right of left face
                glEnd();
                glPopMatrix();
        }

        void genOctreeModel(OctreeNode * node, int depth, vec3 _pos, float _size)
        {
                        
            if (node == nullptr) {
                printf("faile\n");
                return;
            }
            if( node->isLeaf) {//draw?
                
                Cube cube;
                cube.color = node->color;
                cube.pos = _pos;
                cube.size = _size;
                cubes.push_back(cube);
                return;
            }
            for(int i = 0; i < 8; i++) {
                
                vec3 newPos;
                newPos.x = _pos.x + (_size * split[i].x) / 2;
                newPos.y = _pos.y + (_size * split[i].y) / 2;
                newPos.z = _pos.z + (_size * split[i].z) / 2;
                
                /*vec3 newPos = _pos;
                newPos += (split[i] * _size / 2);*/
                //if(distance3D(0,0,0, newPos.x, newPos.y, newPos.z ) < this->size/2) {
                if(node->node)
                    genOctreeModel(&(node->node[i]), depth, newPos, _size/2.0f);
                // }
            }
        }
        void draw() {
            static float rot = 0.0f;
            rot += 0.1f;
            glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            glRotatef(rot, 0.5f, 0.8f, 0.0f);
            for(unsigned int i = 0; i < cubes.size(); i++) {
               // if(distance3D(0,0,0, cubes[i].pos.x, cubes[i].pos.y, cubes[i].pos.z ) < this->size/2) {
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
               //}
              
            }
            glBegin(GL_QUADS);
                        glColor3f(255, 255,255);
                        // Top 
                        glVertex3f( 1,  1, -1);  // Top-right of top face
                        glVertex3f(-1,  1, -1);  // Top-left of top face
                        glVertex3f(-1,  1,  1);  // Bottom-left of top face
                        glVertex3f( 1,  1,  1);  // Bottom-right of top face
                
                        // Bottom face
                        glVertex3f( 1, -1, -1); // Top-right of bottom face
                        glVertex3f(-1, -1, -1); // Top-left of bottom face
                        glVertex3f(-1, -1,  1); // Bottom-left of bottom face
                        glVertex3f( 1, -1,  1); // Bottom-right of bottom face
                        
                        // Front face
                        glVertex3f( 1,  1,  1);  // Top-Right of front face
                        glVertex3f(-1,  1,  1);  // Top-left of front face
                        glVertex3f(-1, -1,  1);  // Bottom-left of front face
                        glVertex3f( 1, -1,  1);  // Bottom-right of front face
                    
                        // Back face
                        glVertex3f( 1, -1, -1); // Bottom-Left of back face
                        glVertex3f(-1, -1, -1); // Bottom-Right of back face
                        glVertex3f(-1,  1, -1); // Top-Right of back face
                        glVertex3f( 1,  1, -1); // Top-Left of back face
                    
                    
                        // Left face
                        glVertex3f(-1,  1,  1);  // Top-Right of left face
                        glVertex3f(-1,  1, -1);  // Top-Left of left face
                        glVertex3f(-1, -1, -1);  // Bottom-Left of left face
                        glVertex3f(-1, -1,  1);  // Bottom-Right of left face
                        // Right face
                        glVertex3f( 1,  1,  1);  // Top-Right of left face
                        glVertex3f( 1,  1, -1);  // Top-Left of left face
                        glVertex3f( 1, -1, -1);  // Bottom-Left of left face
                        glVertex3f( 1, -1,  1);  // Bottom-Right of left face
                    glEnd();
              //  }
            
            glPopMatrix();
        }
        //traverse(depth)
        //

        void outputFile(const char * file)
        {
            std::ofstream out;
            out.open(file);
            if(!out.is_open()) {
                printf("FAILED TO OPEN FILE");
                return;
            }
            std::string output;
            output = std::to_string(this->size) + "\n";
            output += std::to_string(this->maxDepth) + "\n";
            output += octreeToString(root);
            out.write(output.c_str(), output.length());
            
        }
        std::string octreeToString(OctreeNode * node, std::string str = "")
        {
            if (node == nullptr) {
                str = "";
                return str;
            }
            if (node->isLeaf) {
                std::string color = std::to_string(node->color.x);
                color += " " + std::to_string(node->color.y);
                color += " " + std::to_string(node->color.z);
                return str +" "+color+"\n";//+std::to_string()+" " +  + "\n";
            }
        
            std::string ret;
            for (int i = 0; i < 8; i++) {
                if(node->node)
                    ret += octreeToString(&node->node[i], str + to_string(i));
            }
            return ret;
        }
};



#endif
