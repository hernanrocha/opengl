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
GLfloat zrot;									// Z Rotation
GLfloat xspeed = 0.01;                                 // X Rotation Speed
GLfloat yspeed = 0.01;                                 // Y Rotation Speed

BOOL    move = true;
BOOL    desplazar = false;
GLfloat xTraslacion = 0;

GLfloat LightAmbient[] = { 1.0f, 0.5f, 0.5f, 1.0f };				// Ambient Light Values

GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  				// Diffuse Light Values
GLfloat LightPosition[] = { 0.0f, 0.0f, 10.0f, 1.0f };				// Light Position

GLuint  filter;                                 // Which Filter To Use
GLuint  texture[3];                             // Storage for 3 textures

const float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos;

GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;
GLfloat	z=0.0f;				// Depth Into The Screen

Object3D object;

using namespace std;

/* The number of our GLUT window */
int window;
int mirror;

typedef struct tagVERTEX
{
	float x, y, z;
	float u, v;
} VERTEX;

typedef struct tagTRIANGLE
{
	VERTEX vertex[3];
} TRIANGLE;

typedef struct tagSECTOR
{
	int numtriangles;
	TRIANGLE* triangle;
} SECTOR;

SECTOR sector1;				// Our Model Goes Here:

void readstr(FILE *f,char *string)
{
	do
	{
		fgets(string, 255, f);
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;
}

void SetupWorld()
{
	float x, y, z, u, v;
	int numtriangles;
	FILE *filein;
	char oneline[255];
	filein = fopen("data/world.txt", "rt");				// File To Load World Data From

	readstr(filein,oneline);
	sscanf(oneline, "NUMPOLLIES %d\n", &numtriangles);

	sector1.triangle = new TRIANGLE[numtriangles];
	sector1.numtriangles = numtriangles;
	for (int loop = 0; loop < numtriangles; loop++)
	{
		for (int vert = 0; vert < 3; vert++)
		{
			readstr(filein,oneline);
			sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
			sector1.triangle[loop].vertex[vert].x = x;
			sector1.triangle[loop].vertex[vert].y = y;
			sector1.triangle[loop].vertex[vert].z = z;
			sector1.triangle[loop].vertex[vert].u = u;
			sector1.triangle[loop].vertex[vert].v = v;
		}
	}
	fclose(filein);
	return;
}

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
		cout << "Textura no cargada " << endl;									// If Texture Didn't Load Return FALSE
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
	glEnable(GL_LIGHT1);

	// Enable Light One
	SetupWorld();
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
	Width = 640;
	Height = 480;

  if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
    Height=1;

  glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  gluPerspective(45.0f,(GLfloat) 4/3,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

	glLoadIdentity();									// Reset The View

	/*GLfloat x_m, y_m, z_m, u_m, v_m;
	GLfloat xtrans = -xpos;
	GLfloat ztrans = -zpos;
	GLfloat ytrans = -walkbias-0.25f;
	GLfloat sceneroty = 360.0f - yrot;

	int numtriangles;

	glRotatef(lookupdown,1.0f,0,0);
	glRotatef(sceneroty,0,1.0f,0);

	glTranslatef(xtrans, ytrans, ztrans);
	glBindTexture(GL_TEXTURE_2D, texture[filter]);

	numtriangles = sector1.numtriangles;

	// Process Each Triangle
	for (int loop_m = 0; loop_m < numtriangles; loop_m++)
	{
		glBegin(GL_TRIANGLES);
			glNormal3f( 0.0f, 0.0f, 1.0f);
			x_m = sector1.triangle[loop_m].vertex[0].x;
			y_m = sector1.triangle[loop_m].vertex[0].y;
			z_m = sector1.triangle[loop_m].vertex[0].z;
			u_m = sector1.triangle[loop_m].vertex[0].u;
			v_m = sector1.triangle[loop_m].vertex[0].v;
			glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);

			x_m = sector1.triangle[loop_m].vertex[1].x;
			y_m = sector1.triangle[loop_m].vertex[1].y;
			z_m = sector1.triangle[loop_m].vertex[1].z;
			u_m = sector1.triangle[loop_m].vertex[1].u;
			v_m = sector1.triangle[loop_m].vertex[1].v;
			glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);

			x_m = sector1.triangle[loop_m].vertex[2].x;
			y_m = sector1.triangle[loop_m].vertex[2].y;
			z_m = sector1.triangle[loop_m].vertex[2].z;
			u_m = sector1.triangle[loop_m].vertex[2].u;
			v_m = sector1.triangle[loop_m].vertex[2].v;
			glTexCoord2f(u_m,v_m); glVertex3f(x_m,y_m,z_m);
		glEnd();
	}*/

	/*glLoadIdentity();
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
	glEnd();*/

	glLoadIdentity();                           // Reset The Current Matrix
	glTranslatef(xTraslacion,0.0f,-5.0f);                      // Move Into The Screen 5 Units

	glRotatef(xrot,1.0f,0.0f,0.0f);                     // Rotate On The X Axis
	glRotatef(yrot,0.0f,1.0f,0.0f);                     // Rotate On The Y Axis
	glRotatef(zrot,0.0f,0.0f,1.0f);                     // Rotate On The Z Axis

	glBegin(GL_QUADS);
	    // Front Face
	    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
	    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
	    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
	    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
	    // Back Face
	    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
	    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
	    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
	    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
	    // Top Face
	    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
	    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
	    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
	    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
	    // Bottom Face
	    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
	    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
	    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
	    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
	    // Right face
	    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
	    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
	    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
	    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
	    // Left Face
	    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
	    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
	    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
	    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
	glEnd();

	// since this is double buffered, swap the buffers to display what just got drawn.
	glutSwapBuffers();

	//rtri+=0.02f;                     // Increase The Rotation Variable For The Triangle
	//rquad-=0.015f;                   // Decrease The Rotation Variable For The Quad
	if (move){
		xrot+=xspeed;
		yrot+=yspeed;
	}

	if (desplazar){
		xTraslacion -= 0.0001f;
	}
	tRot += 0.01f;

	// Refresh the other window
	glutPostRedisplay();
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
    }else if (key == 'm'){
    	move = !move;
    }else if (key == 't'){
    	desplazar = !desplazar;
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
  glutInitWindowSize(400, 400);

  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(0, 0);
  window = glutCreateWindow("OpenGL Test");

  /* Register the function to do all our OpenGL drawing. */
  glutDisplayFunc(&DrawGLScene);

  /* Go fullscreen.  This is as soon as possible. */
  //glutFullScreen();

  /* Register the function called when our window is resized. */
  glutReshapeFunc(&ReSizeGLScene);

  /* Register the function called when the keyboard is pressed. */
  glutKeyboardFunc(&keyPressed);

  /* Initialize our window. */
  InitGL(640, 480);

  glutInitWindowPosition(400,0);
  mirror = glutCreateWindow("Mirror Window");

  glutDisplayFunc(&DrawGLScene);
  //glutFullScreen();
  glutReshapeFunc(&ReSizeGLScene);
  glutKeyboardFunc(&keyPressed);
  InitGL(640, 480);

  /* Even if there are no events, redraw our gl scene. */
  glutIdleFunc(&DrawGLScene);

  /* Start Event Processing Engine */
  glutMainLoop();

  return 1;
}

