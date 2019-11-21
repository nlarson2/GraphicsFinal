//Main taken from Fate The Shadow Wizard Kingdom Darkness Revived 2 The Second one

//Modified by: Nickolas Larson
//Date:        10/05/2019
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <unistd.h>

#include "Maths.h"
#include "Model.h"
#include "Octree.h"

#include "Image.h"
using namespace std;

/*******************************************************/

void displayCharacterImage(GLuint image, float x, float y) {
    float wid = 50;
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, image);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

class Global {
    public:
        int xres, yres;
        int n;
        int count;
        bool ctrls = true;
        int done = 0;
        bool quit;
        Octree octree;
        static Global * GetInstance()
        {
            if (!instance)
                instance = new Global();

            return instance;
        }
    private:
        static Global * instance;
        Global() : /*octree(6, 4){*/octree(4, "WoodenCabinObj.obj", "ModelTextureEnemy.png", "cabin.txt", 120){/*octree("model.txt") {*/
            xres = 1200;
            yres = 900;
            count = 0;
            quit = false;
            octree.genOctreeModel();
            quit = false;
        }
        Global(Global const& copy);
        Global & operator=(Global const& copy);

};
Global* Global::instance = 0;
Global * g = Global::GetInstance();

class X11_wrapper {
    private:
        Display *dpy;
        Window win;
        GLXContext glc;
    public:
        ~X11_wrapper() {
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);
        }
        X11_wrapper() {
            GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
            int w = g->xres, h = g->yres;
            dpy = XOpenDisplay(NULL);
            if (dpy == NULL) {
                cout << "\n\tcannot connect to X server\n" << endl;
                exit(EXIT_FAILURE);
            }
            Window root = DefaultRootWindow(dpy);
            XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
            if (vi == NULL) {
                cout << "\n\tno appropriate visual found\n" << endl;
                exit(EXIT_FAILURE);
            } 
            Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
            XSetWindowAttributes swa;
            swa.colormap = cmap;
            swa.event_mask =
                ExposureMask | KeyPressMask | KeyReleaseMask |
                ButtonPress | ButtonReleaseMask |
                PointerMotionMask |
                StructureNotifyMask | SubstructureNotifyMask;
            win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
                    InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
            set_title();
            glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
            glXMakeCurrent(dpy, win, glc);
        }
        void set_title() {
            //Set the window title bar.
            XMapWindow(dpy, win);
            XStoreName(dpy, win, "Cubes");
        }
        bool getXPending() {
            //See if there are pending events.
            return XPending(dpy);
        }
        XEvent getXNextEvent() {
            //Get a pending event.
            XEvent e;
            XNextEvent(dpy, &e);
            return e;
        }
        void swapBuffers() {
            glXSwapBuffers(dpy, win);
        }
}x11;

//Function prototypes
void init_opengl(void);
void init_opengl3D(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void render();
//=====================================
// MAIN FUNCTION IS HERE
//=====================================
int main()
{
     
    //g->octree.printOctree();

   // g->quit = true;
    srand(time(NULL));
    init_opengl3D();
    //Main animation loop

    while (!g->quit) {
        //Process external events.
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            check_mouse(&e);
            g->done = check_keys(&e);
        }
        render();
        x11.swapBuffers();
    }
    return 0;
}

void init_opengl(void)
{
   
    //OpenGL initialization
    glViewport(0, 0, g->xres, g->yres);
    //Initialize matrices

    glOrtho(0, g->xres, 0, g->yres, -1, 1);

    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    //Insert Fonts
    glEnable(GL_TEXTURE_2D);
}

void init_opengl3D(void)
{
    //OpenGL initialization
    glViewport(0, 0, g->xres, g->yres);
    //Initialize matrices
    //3D perspective view

    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)g->xres/(GLfloat)g->yres,0.1f,1000.0f);

    glShadeModel(GL_SMOOTH);//enables smooth shading

    // sets the depth buffer//stop elements from drawing over others
    glClearDepth(1.0f);//Depth buffer setup
    glEnable(GL_DEPTH_TEST);//Enables Depth Testing
    glDepthFunc(GL_LEQUAL);//The type of depth test to do

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//??makes the perspective view better??

    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    //Insert Fonts
    glEnable(GL_TEXTURE_2D);
}
void check_mouse(XEvent *e)
{
    static int savex;
    static int savey;
    //int flag;

    if (e->type != ButtonRelease &&
            e->type != ButtonPress) {
        //This is not a mouse event that we care about.
        return;
    }
    if (e->type == ButtonRelease) {
        return;
    }
    //----------------------------------------------------------
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button was pressed
            if(savex != e->xbutton.x || savey != e->xbutton.y) {
                savex = e->xbutton.x;
                savey = e->xbutton.y;
            }
            
            //---------------------------------------------------------
            if (e->xbutton.button==3) {
                //Right button was pressed
                return;
            }
        }
    }
}

int check_keys(XEvent *e)
{
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    int key = XLookupKeysym(&e->xkey, 0); 
    if (e->type == KeyPress) {
        //game.procKeyInput(key);
        if(key == XK_Escape)
            g->quit = true;        
    }
    return 0;
}
void GenerateGLTexture(GLuint & texture, const char * dataSrc, bool inverted)
{
	//IMAGE CLASS NEEDS DEFINITION	
	Image data(dataSrc);
	//flip image data because openGL is inverted on the Y
	if (inverted)
		data.invertY();

	int width = data.width;
	int height = data.height;
	glGenTextures( 1, &texture);
	glBindTexture( GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, data.data);
	glBindTexture( GL_TEXTURE_2D, 0);
}

void render()
{
    usleep(1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    g->octree.draw();
    //g->octree.drawTree();
    //printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
    
    // Placeholder control display while in testing
   /* extern BHglobal bhg;
    if(bhg.ctrls) {
        Rect r;
        glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, g->xres, 0, g->yres, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_LIGHTING);
        r.bot = g->yres - 20;
        r.left = 10;
        r.center = 0;
        ggprint8b(&r, 16, 0xFFFFFFFF, "-----Controls-----");
        ggprint8b(&r, 16, 0xFFFFFFFF, "W - Camera Up");
        ggprint8b(&r, 16, 0xFFFFFFFF, "A - Camera Left");
        ggprint8b(&r, 16, 0xFFFFFFFF, "S - Camera Down");
        ggprint8b(&r, 16, 0xFFFFFFFF, "D - Camera Right");
        ggprint8b(&r, 16, 0xFFFFFFFF, "C - Center On Player");
        ggprint8b(&r, 16, 0xFFFFFFFF, "Esc - Pause Game");

        ggprint8b(&r, 16, 0xFFFFFFFF, "---World Controls---");
        ggprint8b(&r, 16, 0xFFFFFFFF, "Left Click - Move/Interact");

        ggprint8b(&r, 16, 0xFFFFFFFF, "---Battle Controls---");
        ggprint8b(&r, 16, 0xFFFFFFFF, "E - End Turn");
        ggprint8b(&r, 16, 0xFFFFFFFF, "Left Click - Move/Attack");

        ggprint8b(&r, 16, 0xFFFFFFFF, "-----Objective-----");
        ggprint8b(&r, 16, 0xFFFFFFFF, "Defeat all enemies on the map");
        ggprint8b(&r, 16, 0xFFFFFFFF, "Do not let your hero die.");
        ggprint8b(&r, 16, 0xFFFFFFFF, "Heal after battle via towns.");

        glPopMatrix();
    }*/

}
