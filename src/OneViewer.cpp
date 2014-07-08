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
#include <winsock.h>

#include "Object3D.h"


/* ascii code for the escape key */
#define ESCAPE 27
#define WIDTH 1366
#define HEIGHT 768
#define SCREEN_WIDTH 1366

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

// SERVER
#define MAXBUFLEN 512
#define PORT 9799

char RecvBuff[MAXBUFLEN];

struct Viewer {
	GLfloat x, y, z;
};
Viewer viewer;
#define X_DESP 0.1
#define Z_DESP 0.1

struct Screen {
	int id;
	GLdouble centerX, centerY, centerZ;
	GLfloat left, right, bottom, top, dist;
};

struct Monitor {
	int x, y, w, h;
};

AUX_RGBImageRec * loadBMP(char *Filename)                    // Loads A Bitmap Image
{
	FILE *File=NULL;                            // File Handle

	if (!Filename)                              // Make Sure A Filename Was Given
	{
		return NULL;                            // If Not Return NULL
	}

	File=fopen(Filename,"r");                       // Check To See If The File Exists

	if (File)                               // Does The File Exist?
	{
		fclose(File);                           // Close The Handle
		return auxDIBImageLoad(Filename);               // Load The Bitmap And Return A Pointer
	}else{
		cout << "No encontrado " << endl;
	}
	return NULL;                                // If Load Failed Return NULL
}


int loadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]= loadBMP("Data/Crate.bmp"))
	{
		//cout << "Cargar texturas" << endl;

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

		if (TextureImage[0]->data)						// If Texture Image Exists
		{
			free(TextureImage[0]->data);// Free The Texture Image Memory will not compile in dev
		}

		free(TextureImage[0]);							// Free The Image Structure
	}

	return Status;										// Return The Status
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL()	        // We call this right after our OpenGL window is created.
{
	// Jump To Texture Loading Routine
	if (!loadGLTextures()){
		cout << "Textura no cargada " << endl;			// Texture Didn't Load
	}

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Luces
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);

}

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

/* The main drawing function. */
void drawGLScene(int id){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();									// Reset The View

	if(id == 3){
		// SCREEN 3
		gluLookAt(viewer.x, viewer.y, viewer.z, (viewer.x - 5.0f), viewer.y, viewer.z, 0.0f, 1.0f, 0.0f); // Tg(22.5º) = OP
		//glTranslatef(-3.0f, 0.0f, 1.0f);
	}else{
		// SCREEN 1-2
		gluLookAt(viewer.x, viewer.y, viewer.z, viewer.x, viewer.y, (viewer.z - 5.0f), 0.0f, 1.0f, 0.0f); // Tg(22.5º) = OP
	}

	if (move){
		xrot+=xspeed;
		yrot+=yspeed;
	}

	//if (move){
	glRotatef(xrot,1.0f,0.0f,0.0f);                     // Rotate On The X Axis
	glRotatef(yrot,0.0f,1.0f,0.0f);                     // Rotate On The Y Axis
	//glRotatef(zrot,0.0f,0.0f,1.0f);                     // Rotate On The Z Axis
	//}else if (desplazar){
	//	glTranslatef(xTraslacion,0.0f,0.0f);                      // Move Into The Screen 5 Units
	//}

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


	glutPostRedisplay();

	/*if (desplazar){
		xTraslacion -= 0.0001f;
	}
	tRot += 0.01f;*/
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
		desplazarMirror = desplazar;
	}else if (key == 'd'){
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glOrtho(0.0f, 4.0f, -2.0f, 2.0f, -5.0f, 5.0f);
		viewer.x += X_DESP;
	}else if (key == 'a'){
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glOrtho(-4.0f, 0.0f, -2.0f, 2.0f, -5.0f, 5.0f);
		viewer.x -= X_DESP;
	}else if (key == 's'){
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glOrtho(-2.0f, 2.0f, -2.0f, 2.0f, -5.0f, 5.0f);
		viewer.z += Z_DESP;
	}else if (key == 'w'){
		viewer.z -= Z_DESP;
	}else {
		cout << (int)key << endl;
	}
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitRight()	        // We call this right after our OpenGL window is created.
{
	// Jump To Texture Loading Routine
	if (!loadGLTextures()){
		cout << "Textura no cargada " << endl;			// Texture Didn't Load
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

	// Matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// (A) Proyeccion Ortografica
	//glOrtho(-2.0f, 2.0f, -2.0f, 2.0f, -5.0f, 5.0f); // Camara Central
	//glOrtho(0.0f, 4.0f, -2.0f, 2.0f, -5.0f, 5.0f);	// Camara Derecha
	//glOrtho(-4.0f, 0.0f, -2.0f, 2.0f, -5.0f, 5.0f); // Camara Izquierda

	// (B) Proyeccion Perspectiva (distintos COP)
	// Perspectiva: FOV = 45º, Cuadrado, Desde 0.1f a 100.0f
	//gluPerspective(45.0f, 1.0f, 0.1f, 100.0f);
	//gluLookAt(1.6568f, 0.0f, 5.0f, 1.6568f, 0.0f, 3.0f, 0.0f, 1.0f, 0.0f);

	// (C) Proyeccion Perspectiva (mismo COP)
	//gluPerspective(45.0f, 1.0f, 0.1f, 100.0f);
	//gluLookAt(0.0f, 0.0f, 5.0f, 0.4142f, 0.0f, 4.0f, 0.0f, 1.0f, 0.0f); // Tg(22.5º) = OP

	// (D) Proyeccion Dos pantallas
	//gluPerspective(90.0f, 1.0f, 0.1f, 100.0f);
	//gluLookAt(0.0f, 0.0f, 3.0f, 1.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f); // Tg(22.5º) = OP

	// (E) Proyeccion Perspectiva (mismo COP)
	//gluPerspective(45.0f, 1.0f, 0.1f, 100.0f);
	glFrustum(0.0f, 2.0f, -1.0f, 1.0f, 2.0f, 50.0f);
	gluLookAt(-2.0f, 0.0f, 8.0f, 0.0f, 0.0f, 3.0f, 0.0f, 1.0f, 0.0f); // Tg(22.5º) = OP

	// Matriz de transformacion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void initScreen(Screen scr, Monitor mon){
	// ------------------------ LEFT WINDOW ------------------------- //

	glutInitWindowPosition(mon.x, mon.y);
	glutInitWindowSize(mon.w, mon.h);
	window = glutCreateWindow("Viewer Test 1");

	//glutDisplayFunc(&drawGLScene);
	//glutIdleFunc(&drawGLScene);
	if (scr.id == 3){
		glutDisplayFunc([](){
			drawGLScene(3);
		});
		glutIdleFunc([](){
			drawGLScene(3);
		});
	}else{
		glutDisplayFunc([](){
			drawGLScene(1);
		});
		glutIdleFunc([](){
			drawGLScene(1);
		});
	}
	glutReshapeFunc(&resizeGLScene);
	glutFullScreen();
	glutKeyboardFunc(&keyPressed);

	InitGL();

	// Matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(scr.left / 10.0f,
			  scr.right / 10.0f,
			  scr.bottom / 10.0f,
		  	  scr.top / 10.0f,
			  scr.dist / 10.0f,
			  50.0f);

	// Matriz de transformacion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

int initServidor(){

	WSADATA wsaData;
	SOCKET conn_socket,comm_socket;
	SOCKET comunicacion;
	struct sockaddr_in server;
	struct sockaddr_in client;
	struct hostent *hp;
	int resp,stsize;
	u_short port;
	hostent* localHost;
	char* localIP;

	//Inicializamos la DLL de sockets
	resp = WSAStartup(MAKEWORD(2,0),&wsaData);
	if(resp){
		printf("Error al inicializar socket\n");
		getchar();
		return resp;
	}

	// Obtenemos la IP que usará nuestro servidor...
	// en este caso localhost indica nuestra propia máquina...
	hp = gethostbyname("localhost");
	port = htons(PORT);

	if(!hp){
		printf("No se ha encontrado servidor...\n");
		getchar();
		WSACleanup();
		return WSAGetLastError();
	}

	// Creamos el socket...
	conn_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(conn_socket==INVALID_SOCKET) {
		printf("Error al crear socket\n");
		getchar();
		WSACleanup();
		return WSAGetLastError();
	}else {
		cout << "Socket creado.. " << endl;
	}

	// Configurar servidor
	localHost = gethostbyname("");
	localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);
	server.sin_family=AF_INET; //
	server.sin_addr.s_addr=INADDR_ANY; //
	server.sin_addr.s_addr = inet_addr (localIP);
	server.sin_port = htons(PORT);
	bind(conn_socket,(SOCKADDR*)&server,sizeof(server));

	// Abrir Servidor
	listen(conn_socket, 8);
	cout << "Servidor en escucha " << endl;

	// Esperar cliente
	int len = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket,(sockaddr*)&client,&len);

	while (true){
		// Recibir informacion
		int i;
		char buffer[1025];
		i= recv(comm_socket,buffer,1024,0);

		// Enviar contestacion
		char bufferSalida[] = "Hola, Saludos!";
		int enviado = send(comm_socket,bufferSalida,strlen(bufferSalida),0);

		// Se visualiza lo recibido
		//RecvBuff[resp] = '\0';
		cout << "Paquete proveniente de: " << inet_ntoa(client.sin_addr) << endl;
		cout << "Longitud del paquete en bytes: " << i << "..." << endl;
		cout << "El paquete contiene: " << buffer << endl;

		getchar();
	}

	// Cerramos el socket de comunicacion
	closesocket(conn_socket);

	return 0;

}

int main(int argc, char **argv) {
	//int serv = initServidor();

	// CLIENTE
	/*client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	client.sin_addr = *((struct in_addr*)hp->h_addr);
	connect(conn_socket,(sockaddr*)&client, sizeof(sockaddr));*/

	// Cerramos liberia winsock
	//WSACleanup();
	//return (EXIT_SUCCESS);


	// Initialize GLUT state
	glutInit(&argc, argv);

	/* Select type of Display mode:
     Double buffer
     RGBA color
     Alpha components supported
     Depth buffer */
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	// Viewer
	viewer.x = 0.0f;
	viewer.y = 0.0f;
	viewer.z = 8.0f;

	// Screen 1
	// ID: 1
	// Centro: (0,0,-5)
	// AnchoReal(en dm): (-3,316, 0)
	Screen scr1;
	scr1.id = 1;
	scr1.centerX = 0.0f;
	scr1.centerY = 0.0f;
	scr1.centerZ = -5.0f;
	scr1.left = -3.316f;
	scr1.right = 0.0f;
	scr1.bottom = -0.9321f;
	scr1.top = 0.9321f;
	scr1.dist = 5.0f;

	// Screen 2
	Screen scr2;
	scr2.id = 2;
	scr2.left = 0.0f;
	scr2.right = 3.316f;
	scr2.bottom = -0.2321f;
	scr2.top = 1.6321f;
	scr2.dist = 5.0f;

	// Screen 3
	Screen scr3;
	scr3.id = 3;
	scr3.left = 1.35f;
	scr3.right = 5.0f;
	scr3.bottom = -0.9321f;
	scr3.top = 0.9321f;
	scr3.dist = 3.316f;

	// Monitor 1
	Monitor mon1;
	mon1.x = 0;
	mon1.y = 0;
	mon1.w = WIDTH;
	mon1.h = HEIGHT;

	initScreen(scr1, mon1);
	//initScreen(scr2, SCREEN_WIDTH, 0.0f, WIDTH, HEIGHT);
	//initScreen(scr3, SCREEN_WIDTH, 0.0f, WIDTH, HEIGHT);

	// ------------------------ RIGHT WINDOW ------------------------- //

	/*glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(SCREEN_WIDTH, 0);
	window = glutCreateWindow("Viewer Test 2");

	glutDisplayFunc(&drawGLScene);
	glutIdleFunc(&drawGLScene);
	//glutReshapeFunc(&resizeGLScene);
	glutFullScreen();
	glutKeyboardFunc(&keyPressed);

	InitRight();*/

	glutMainLoop();

	return 1;
}

