#ifndef OCTREE_H
#define OCTREE_H

#include <GL/glu.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <pthread.h>


#include "Maths.h"
#include "Model.h"

using namespace std;

struct rgb {
    int r, g, b;
};

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
    bool edge;
    OctreeNode(){
        isLeaf = false;
    }
    OctreeNode(int _level)
    {
        level = _level;
    }
};

struct tPass {
    OctreeNode * node;
    int level;
    vec3 _pos;
    float _size;
    int maxDepth;
    Model * model;
};
void *proceed(void*);
class Octree{
    private:
        int maxDepth;
        int depth;
        OctreeNode * root;
        vec3 pos;
        float size;
        float radius;
        vector<Cube> cubes;
        Model * model;
    public:
        Octree(int _maxDepth, int _size)
        {
            radius = _size/2;
            pos.x = -0.0f;
            pos.y = -0.0f;
            pos.z = -10.0f;
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
        Octree(int _maxDepth, const char * _model, const char * _texture, const char * outFile, float _size)
        {
            pos.x = -0.0f;
            pos.y = -0.0f;
            pos.z = -20.0f - _size;
            srand(time(nullptr));
            model = new Model(_model, "ModelTextureEnemy.png");
            //model = new Model("WoodenCabinObj.obj", "ModelTextureEnemy.png");
            size = _size;//4;//model->getMaxDim();
            printf("Size: %f\n", size);
            root = new OctreeNode();
            root->level = 0;
            root->edge = false;
            maxDepth = _maxDepth;
            
            //addNodes(root);
            //addSphere(&root, size, vec3(0,0,0));
            //for(int i = 1; i <= maxDepth; i++) {
               // addNodes(root, i);
            
               addNodesModel(root, 0, vec3(0,0,0), size/2.0f);
            //}
            //printOctree();
            outputFile(outFile);
        }


        Octree(const char * voxelFile)
        {
            ifstream in;
            in.open(voxelFile);
            if(!in.is_open()) {
                printf("FAILED\n");
                exit(-1);
            }

            std::string line;
            in >> line;
            size = 20;//stoi(line);
            in >> line;
            maxDepth = stoi(line);


            root = new OctreeNode();
            root->level = 0;
            root->edge = false;
            pos.x = -0.0f;
            pos.y = -0.0f;
            pos.z = -120.0f;
            srand(time(nullptr));
            while(!in.eof()) {
                string path;
                rgb color;
                in >> path;
                //printf("%s ", path.c_str());
                in >> line;
                color.r = 195;//rand()%255;//stoi(line);
                //printf("%d ", color.r);
                in >> line;
                color.g = 195;// rand()%255;// stoi(line);
                //printf("%d ", color.g);
                in >> line;
                color.b = 195;// rand()%255;// stoi(line);
                //printf("%d \n", color.b);
                
                //sscanf(line.c_str(),"%s %i %i %i\n", tempPath, &color.r, &color.g, &color.b);
                //path = *(new std::string(tempPath));
                //printf("%d\n", path.length());
                OctreeNode * pnode = root;
                for(unsigned int i = 0; i < path.length(); i++) {
                    if (pnode->node == nullptr) {
                        pnode->node = new OctreeNode[8];
                        for(int j = 0; j < 8; j++) {
                            pnode->node[j].isLeaf = false;
                            pnode->node[j].edge = true;
                            pnode->node[j].level = i+1;
                        }
                    }
                    int index = path[i] - 48;
                    //printf("%d ", index);
                    pnode = &pnode->node[index];
                    pnode->edge = false;
                }
               // printf("\n");
                pnode->color.x = color.r;
                pnode->color.y = color.g;
                pnode->color.z = color.b;
                pnode->isLeaf = true;

            }
            in.close();
           // printOctree();
            //exit(-1);



            //exit(0);

            
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

                    vec3 newPos;
                    newPos.x = _pos.x + (_size * split[i].x) / 2;
                    newPos.y = _pos.y + (_size * split[i].y) / 2;
                    newPos.z = _pos.z + (_size * split[i].z) / 2;
                    if(distance3DCube(0,0,0, newPos.x, newPos.y, newPos.z, _size/2.0f) < 1.8f) {
                        node->node[i].node = nullptr;
                        node->node[i].color = vec3(rand()%255, rand()%255, rand()%255);
                        node->node[i].level = level;
                        node->node[i].isLeaf = true;
                    } else {
                        node->node[i].node = nullptr;
                        node->node[i].isLeaf = false;
                    }
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
        
        pthread_t th[8];
        
        void * addNodesModel(OctreeNode * node, int level, vec3 _pos, float _size) {

          
            if (node->node == nullptr && level <= this->maxDepth) {
                node->node = new OctreeNode[8];
                node->isLeaf = false;
                for(int i = 0; i < 8; i++) {
                    vec3 newPos;
                    newPos.x = _pos.x + (_size * split[i].x) / 2;
                    newPos.y = _pos.y + (_size * split[i].y) / 2;
                    newPos.z = _pos.z + (_size * split[i].z) / 2;
                    node->node[i].node = nullptr;
                    if(model->checkCubeCollision(newPos, _size/2)) {
                        node->node[i].color = /*vec3(150,150,150);//*/vec3(rand()%255, rand()%255, rand()%255);
                        node->node[i].level = level;
                        node->node[i].isLeaf = true;
                        node->node[i].edge = false;
                        tPass * p = new tPass();
                        p->node = &node->node[i];
                        p->level = level + 1;
                        p->_pos = newPos;
                        p->_size = _size/2;
                        p->maxDepth = maxDepth;
                        p->model = this->model;
                        int ret = pthread_create(&th[i], nullptr, addNodesModel, (void*)(tPass*)p);
                        //addNodesModel( &node->node[i], level + 1, newPos, _size/2);
                    } else {
                        node->node[i].isLeaf = false;
                        node->node[i].edge = true;
                    }
                    //printf("OCTREE LEVEL: %d created\n", level);
                }
                for(int i =0; i < 8; i++)
                        pthread_join(th[i], nullptr);
                
                return nullptr;
            } else {
                
            }

            /*  
            for(int i = 0; i < 8; i++) {

                vec3 newPos;
                newPos.x = _pos.x + (_size * split[i].x) / 2;
                newPos.y = _pos.y + (_size * split[i].y) / 2;
                newPos.z = _pos.z + (_size * split[i].z) / 2;
                //node->node[i].isLeaf = false;
                
                //vec3 newPos = _pos;
                //newPos += (split[i] * _size / 2);
                if(model->checkCubeCollision(newPos, _size/2)) {
           //         printf("So far so good\n");
                    addNodesModel(&node->node[i], level, newPos, _size/2.0f);    
                    //genOctreeModel(&(node->node[i]), depth, newPos, _size/2.0f);
                }
            }*/
        }
        
        static void * addNodesModel(void * t) {
            tPass * p = (tPass*)t;
            pthread_t th[8];
            if (p->node->node == nullptr && p->level <= p->maxDepth) {
                p->node->node = new OctreeNode[8];
                p->node->isLeaf = false;
                for(int i = 0; i < 8; i++) {
                    vec3 newPos;
                    newPos.x = p->_pos.x + (p->_size * split[i].x) / 2;
                    newPos.y = p->_pos.y + (p->_size * split[i].y) / 2;
                    newPos.z = p->_pos.z + (p->_size * split[i].z) / 2;
                    p->node->node[i].node = nullptr;
                    if(p->model->checkCubeCollision(newPos, p->_size/2)) {
                        p->node->node[i].color = vec3(150,150,150);//vec3(rand()%255, rand()%255, rand()%255);
                        p->node->node[i].level = p->level;
                        p->node->node[i].isLeaf = true;
                        p->node->node[i].edge = false;
                        tPass p2;
                        p2.node = &p->node->node[i];
                        p2.level = p->level + 1;
                        p2._pos = newPos;
                        p2._size = p->_size/2;
                        p2.maxDepth = p->maxDepth;
                        p2.model = p->model;
                        
                       //int ret = pthread_create(&th[i], nullptr, addNodesModel, (void*) &p2);
                        Octree::addNodesModel((void*)&p2);
                        //pthread_join(th[i], nullptr);
                    } else {
                        p->node->node[i].isLeaf = false;
                        p->node->node[i].edge = true;
                    }
                    //printf("OCTREE LEVEL: %d created\n", level);
                }
                
                return nullptr;
            } else {
                
            }

            /*  
            for(int i = 0; i < 8; i++) {

                vec3 newPos;
                newPos.x = _pos.x + (_size * split[i].x) / 2;
                newPos.y = _pos.y + (_size * split[i].y) / 2;
                newPos.z = _pos.z + (_size * split[i].z) / 2;
                //node->node[i].isLeaf = false;
                
                //vec3 newPos = _pos;
                //newPos += (split[i] * _size / 2);
                if(model->checkCubeCollision(newPos, _size/2)) {
           //         printf("So far so good\n");
                    addNodesModel(&node->node[i], level, newPos, _size/2.0f);    
                    //genOctreeModel(&(node->node[i]), depth, newPos, _size/2.0f);
                }
            }*/
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
                        
            if (node == nullptr){// || node->edge) {
                printf("faile\n");
                return;
            }
            if( node->isLeaf && !node->edge) {//draw?
                
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
                    genOctreeModel(&node->node[i], depth+1, newPos, _size/2.0f);
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
              
            }/*
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
              //  }*/
            
            glPopMatrix();
           // model->draw(pos.x, pos.y, pos.z);
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
            output = std::to_string((int)this->size) + "\n";
            output += std::to_string((int)this->maxDepth) + "\n";
            output += octreeToString(root);
            out.write(output.c_str(), output.length()-1);
            
        }
        
        std::string octreeToString(OctreeNode * node, std::string str = "")
        {
            if (node == nullptr || node->edge) {
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
