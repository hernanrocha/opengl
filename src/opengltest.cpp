//
// This code was created by Jeff Molofee '99 (ported to Linux/GLUT by Richard Campbell '99)
//
// If you've found this code useful, please let me know.
//
// Visit me at www.demonews.com/hosted/nehe
// (email Richard Campbell at ulmont@bellsouth.net)
//
#include <GL/glut.h>    // Header File For The GLUT Library
#include <GL/GLAux.h>
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

BOOL    light = true;                                  // Lighting ON / OFF
BOOL    lp;                                 // L Pressed?
BOOL    fp;                                 // F Pressed?

GLfloat xrot;                                   // X Rotation
GLfloat yrot;                                   // Y Rotation
GLfloat xspeed = 0.01;                                 // X Rotation Speed
GLfloat yspeed = 0.01;                                 // Y Rotation Speed
GLfloat z=-5.0f;                                // Depth Into The Screen

GLfloat LightAmbient[] = { 1.0f, 0.5f, 0.5f, 1.0f };				// Ambient Light Values

GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  				// Diffuse Light Values
GLfloat LightPosition[] = { 0.0f, 0.0f, 10.0f, 1.0f };				// Light Position

GLuint  filter;                                 // Which Filter To Use
GLuint  texture[3];                             // Storage for 3 textures

Object3D object;

using namespace std;

/* The number of our GLUT window */
int window;

AUX_RGBImageRec *LoadBMP(char *Filename)                    // Loads A Bitmap Image
{
    FILE *File=NULL;                            // File Handle

    if (!Filename)                              // Make Sure A Filename Was Given
    {
        return NULL;                            // If Not Return NULL
    }

    File=fopen(Filename,"r");                       // Check To See If The File Exists

    if (File)                               // Does The File Exist?
    {
    	cout << "Existe" << endl;
        fclose(File);                           // Close The Handle
        return auxDIBImageLoad(Filename);               // Load The Bitmap And Return A Pointer
    }else{
        cout << "No encontrado " << endl;
    }
    return NULL;                                // If Load Failed Return NULL
}

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP("Data/Crate.bmp"))
	{
		cout << "Cargar texturas" << endl;

		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(3, &texture[0]);					// Create Three Textures

		// Create Nearest Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		// Create Linear Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}

	if (TextureImage[0])								// If Texture Exists
	{
		cout << "Existia..." << endl;

		if (TextureImage[0]->data)						// If Texture Image Exists
		{
			free(TextureImage[0]->data);// Free The Texture Image Memory will not compile in dev
		}

		free(TextureImage[0]);							// Free The Image Structure
	}

	return Status;										// Return The Status
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
	if (!LoadGLTextures())								// Jump To Texture Loading Routine
		{
			cout << "No cargadooo " << endl;									// If Texture Didn't Load Return FALSE
		}

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One
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
	xrot+=xspeed;
	yrot+=yspeed;
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
    }else if (key == 'l'){
    	cout << "Cambiando.." << key << endl;
    	light=!light;               // Toggle Light TRUE/FALSE
    	if (!light)             // If Not Light
    	{
    		glDisable(GL_LIGHTING);     // Disable Lighting
    	}
    	else                    // Otherwise
    	{
    		glEnable(GL_LIGHTING);      // Enable Lighting
    	}
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
  //glutFullScreen();

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

