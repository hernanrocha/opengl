//
// This code was created by Jeff Molofee '99 (ported to Linux/GLUT by Richard Campbell '99)
//
// If you've found this code useful, please let me know.
//
// Visit me at www.demonews.com/hosted/nehe
// (email Richard Campbell at ulmont@bellsouth.net)
//
#include <GL/glut.h>    // Header File For The GLUT Library
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <unistd.h>     // Header file for sleeping.
#include <time.h>

#include <iostream>
#include <iomanip>
#include <fstream>

#include "Object3D.h"

/* ascii code for the escape key */
#define ESCAPE 27

bool        keys[256];             // Array Used For The Keyboard Routine
bool        active=TRUE;           // Window Active Flag
bool        fullscreen=TRUE;       // Fullscreen Flag Set To TRUE By Default

GLfloat     rtri;                  // Angle For The Triangle
GLfloat     rquad;                 // Angle For The Quad
GLfloat     tRot;

BOOL    light;                                  // Lighting ON / OFF
BOOL    lp;                                 // L Pressed?
BOOL    fp;                                 // F Pressed?

GLfloat xrot;                                   // X Rotation
GLfloat yrot;                                   // Y Rotation
GLfloat xspeed;                                 // X Rotation Speed
GLfloat yspeed;                                 // Y Rotation Speed
GLfloat z=-5.0f;                                // Depth Into The Screen

GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };				// Ambient Light Values

GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  				// Diffuse Light Values
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };				// Light Position

Object3D object;

using namespace std;

/* The number of our GLUT window */
int window;

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
  glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
  glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				// Reset The Projection Matrix

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

  glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
    Height=1;

  glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

	// Draw a Triangle
	glLoadIdentity();				// Reset The View
	glTranslatef(-1.5f,0.0f,-6.0f);		// Move Left 1.5 Units And Into The Screen 6.0
	glRotatef(rtri,0.0f,1.0f,0.0f);             // Rotate The Triangle On The Y axis

	glBegin(GL_POLYGON);				// start drawing a polygon
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f( 0.0f, 1.0f, 0.0f);		// Top
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left
	glEnd();					// we're done with the polygon

	// Draw a Square (quadrilateral)
	glLoadIdentity();                   // Reset The Current Modelview Matrix
	glTranslatef(1.5f,0.0f,-6.0f);              // Move Right 1.5 Units And Into The Screen 6.0
	glRotatef(45.0f,0.0f,0.0f,-1.0f);            // Rotate The Quad On The X axis

	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);				// start drawing a polygon (4 sided)
		glVertex3f(-1.0f, 1.0f, 0.0f);		// Top Left
		glVertex3f( 1.0f, 1.0f, 0.0f);		// Top Right
		glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
		glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left
	glEnd();					// done with the polygon

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -20.0f);
	glRotatef(tRot, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
		vector<ElementGroup> groups = object.getGroups();
		map<int, Vertex3D> coordinates = object.getCoordinates();

		int index = 1;
		for (vector<ElementGroup>::iterator i = groups.begin(); i != groups.end(); ++i, ++index){
			ElementGroup& group = *i;
			int count = group.getCount();

			vector<Incidence> elements = group.getElements();
			for (vector<Incidence>::iterator j = elements.begin(); j != elements.end(); ++j){
				Incidence& element = *j;
				int a = element.getA();
				int b = element.getB();
				int c = element.getC();

				Vertex3D v = coordinates.find(a)->second;
				glVertex3d(v.x, v.y, v.z);
				v = coordinates.find(b)->second;
				glVertex3d(v.x, v.y, v.z);
				v = coordinates.find(c)->second;
				glVertex3d(v.x, v.y, v.z);

			}
		}
		//gl
	glEnd();

	/*if (keys[VK_F1])                // Is F1 Being Pressed?
	{
	    keys[VK_F1]=FALSE;          // If So Make Key FALSE
	    KillGLWindow();             // Kill Our Current Window
	    fullscreen=!fullscreen;         // Toggle Fullscreen / Windowed Mode
	    // Recreate Our OpenGL Window ( Modified )
	    if (!CreateGLWindow("NeHe's First Polygon Tutorial",640,480,16,fullscreen))
	    {
	        return 0;           // Quit If Window Was Not Created
	    }
	}*/

	// since this is double buffered, swap the buffers to display what just got drawn.
	glutSwapBuffers();

	//rtri+=0.02f;                     // Increase The Rotation Variable For The Triangle
	//rquad-=0.015f;                   // Decrease The Rotation Variable For The Quad
	tRot += 0.01f;
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y)
{
    /* avoid thrashing this procedure */
    usleep(100);

    /* If escape is pressed, kill everything. */
    if (key == ESCAPE){
		/* shut down our window */
		glutDestroyWindow(window);

		/* exit the program...normal termination. */
		exit(0);
    }
}

void loadObjects(){
	ifstream objectFile;

	objectFile.open("cubo.sur");
	objectFile >> object;
	objectFile.close();

}

int main(int argc, char **argv) {

	loadObjects();
	srand(time(NULL));

  /* Initialize GLUT state - glut will take any command line arguments that pertain to it or
     X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */
  glutInit(&argc, argv);

  /* Select type of Display mode:
     Double buffer
     RGBA color
     Alpha components supported
     Depth buffer */
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

  /* get a 640 x 480 window */
  //glutInitWindowSize(640, 480);

  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(0, 0);

  /* Open a window */
  window = glutCreateWindow("OpenGL Test");

  /* Register the function to do all our OpenGL drawing. */
  glutDisplayFunc(&DrawGLScene);

  /* Go fullscreen.  This is as soon as possible. */
  glutFullScreen();

  /* Even if there are no events, redraw our gl scene. */
  glutIdleFunc(&DrawGLScene);

  /* Register the function called when our window is resized. */
  glutReshapeFunc(&ReSizeGLScene);

  /* Register the function called when the keyboard is pressed. */
  glutKeyboardFunc(&keyPressed);

  /* Initialize our window. */
  InitGL(640, 480);

  /* Start Event Processing Engine */
  glutMainLoop();

  return 1;
}

