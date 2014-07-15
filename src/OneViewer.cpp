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
#include <stdio.h>
#include <windows.h>
//#include <winsock.h>
#include <math.h>
#include <string>

#include "Object3D.h"
#include "Client.h"

// ENET
#include "enet/enet.h"

/* ascii code for the escape key */

bool        keys[256];             // Array Used For The Keyboard Routine
bool        active=TRUE;           // Window Active Flag
bool        fullscreen=TRUE;       // Fullscreen Flag Set To TRUE By Default

GLfloat     rtri;                  // Angle For The Triangle
GLfloat     rquad;                 // Angle For The Quad
GLfloat     tRot;

BOOL    light = true;                                  // Lighting ON / OFF
BOOL    lp;                                 // L Pressed?
BOOL    fp;                                 // F Pressed?

GLfloat zrot;									// Z Rotation
GLfloat xspeed = 0.01;                                 // X Rotation Speed
GLfloat yspeed = 0.01;                                 // Y Rotation Speed
GLfloat xrot = xspeed;                                   // X Rotation
GLfloat yrot = yspeed;                                   // Y Rotation

BOOL    move = false;
BOOL    desplazar = false;
GLfloat xTraslacion = 0.0001f;
BOOL    desplazarMirror = false;
GLfloat xMirror = 0;

GLfloat LightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };				// Ambient Light Values
GLfloat LightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  				// Diffuse Light Values
GLfloat LightEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };				// Emission Light Values
GLfloat LightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };				// Specular Light Values
GLfloat LightPosition[] = { 10.0f, 10.0f, 10.0f, 1.0f };			// Light Position

GLuint  filter;                                 // Which Filter To Use
GLuint  texture[3];                             // Storage for 3 textures

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

//Viewer viewer;
//map<int, Screen> screens;

//Client client;

// ------------------------ OBJ FILE ------------------------- //
#define FILE_NAME  "foot.obj"							// This is the 3D file we will load.
#define MAX_TEXTURES 100								// The maximum amount of textures to load
#include "Obj.h"
UINT g_Texture[MAX_TEXTURES] = {0};						// This holds the texture info, referenced by an ID

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void resizeGLScene(int Width, int Height)
{
	if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
		Height=1;

	/*glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
	gluPerspective(45.0f,(GLfloat) 4/3,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);*/

	cout << Width << " x " << Height << endl;
}

char ** argumentos;

bool isServer(){
	return (strcmp(argumentos[1], "server") == 0);
}

bool isClient(){
	return (strcmp(argumentos[1], "client") == 0);
}

bool isTest(){
	return (strcmp(argumentos[1], "test") == 0);
}

int main(int argc, char **argv) {
	// Initialize GLUT state
	glutInit(&argc, argv);
	argumentos = argv;

	/* Select type of Display mode:
     Double buffer
     RGBA color
     Alpha components supported
     Depth buffer */
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	// Viewer (X, Y, Z)
	setViewer(0.0f, 0.0f, 5.0f);

	// Monitores
	setupMonitor();

	// Pantallas
	//setupScreen();

	// Archivo INI
	loadINI();

	if (argc < 2){
		cout << "No hay MODE como primer argumento" << endl;
		cout << "Uso: server, client o test" << endl;
		argumentos[1] = "test";
	}

	if (argc < 3){
		cout << "No hay Servidor como segundo argumento. IP local por defecto" << endl;
		setServer("");
	}else {
		char * ip = argumentos[2];
		//u_short port = atoi(argumentos[3]);
		setServer(ip);
	}

	if (enet_initialize () != 0){
		cout << "An error occurred while initializing ENet." << endl;
		return EXIT_FAILURE;
	}else{
		cout << "ENet iniciado OK" << endl;
	}

	if(isTest()){
		// Servidor y Cliente (TEST)
		initScreen(5, true);
		initScreen(6, true);
		//initServer();
		//Sleep(2000);
		//initClient();
		//Sleep(2000);

		initEnetServer();
		//initEnetClient();
		//Sleep(10000);
	}else if (isServer()){
		// Servidor
		initScreen(2, true);
		//initServer();
		initEnetServer();
	}else if(isClient()){
		// Cliente
		initScreen(1, false);
		//initClient();
		initEnetClient();
	}

	glutMainLoop();

	// ------------------------ OBJ FILE ------------------------- //

	//objTest();

	//cout << strcmp("a", "a") << endl;
	//cout << strcmp("a", "b") << endl;

	/*while (true){
		cout << "A: " << getchar() << endl;
	}*/

	return 1;
}
