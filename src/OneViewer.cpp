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
#include <math.h>
#include <string>

#include "Object3D.h"
#include "Client.h"

//#include "boost/thread/thread.hpp"

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


#include "Graphics.h"

//Viewer viewer;
//map<int, Screen> screens;

//Client client;

// ------------------------ OBJ FILE ------------------------- //
#define FILE_NAME  "foot.obj"							// This is the 3D file we will load.
#define MAX_TEXTURES 100								// The maximum amount of textures to load
#include "Obj.h"
UINT g_Texture[MAX_TEXTURES] = {0};						// This holds the texture info, referenced by an ID

/*void publishAction(int action){
	processAction(action);

}*/

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

/*void drawSnowMan() {

	glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);

// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Nose
	glColor3f(1.0f, 0.5f , 0.5f);
	glutSolidCone(0.08f,0.5f,10,2);
}

void snowTest(){
	// Draw ground
	glColor3f(0.0f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f,  100.0f);
	glVertex3f( 100.0f, 0.0f,  100.0f);
	glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();

	// Draw 36 SnowMen
	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0,0,j * 10.0);
			drawSnowMan();
			glPopMatrix();
		}
}*/

/* The main drawing function. */
/*void drawGLScene(int windowID){
	Screen scr = screens[windowID];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();									// Reset The View
	//viewer.yAngle += 0.01;
	glRotatef(-viewer.yAngle, 0.0f, 1.0f, 0.0f);
	gluLookAt(viewer.x, viewer.y + 1, viewer.z,
			(viewer.x + scr.centerX), (viewer.y + 1 + scr.centerY), (viewer.z + scr.centerZ),
			0.0f, 1.0f, 0.0f);

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
	//}*/

	/*glBegin(GL_QUADS);
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
	glEnd();*/

	//snowTest();

	// since this is double buffered, swap the buffers to display what just got drawn.
	//glutSwapBuffers();

	//rtri+=0.02f;                     // Increase The Rotation Variable For The Triangle
	//rquad-=0.015f;                   // Decrease The Rotation Variable For The Quad


	//glutPostRedisplay();

	/*if (desplazar){
		xTraslacion -= 0.0001f;
	}
	tRot += 0.01f;*/
//}

/* The function called whenever a key is pressed. */
/*void keyPressed(unsigned char key, int x, int y)
{
	// avoid thrashing this procedure
	usleep(100);

	// If escape is pressed, kill everything.
	if (key == ESCAPE){
		// shut down our window
		glutDestroyWindow(glutGetWindow());
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
	}else if (key == MOVE_RIGHT){
		//viewer.x += X_DESP * cos(degToRad(viewer.yAngle));
		//viewer.z -= X_DESP * sin(degToRad(viewer.yAngle));
		publishAction(ACTION_MOVE_RIGHT);
	}else if (key == MOVE_LEFT){
		//viewer.x -= X_DESP * cos(degToRad(viewer.yAngle));
		//viewer.z += X_DESP * sin(degToRad(viewer.yAngle));
		publishAction(ACTION_MOVE_LEFT);
	}else if (key == MOVE_DOWN){
		//viewer.x += Z_DESP * sin(degToRad(viewer.yAngle));
		//viewer.z += Z_DESP * cos(degToRad(viewer.yAngle));
		publishAction(ACTION_MOVE_DOWN);
	}else if (key == MOVE_UP){
		//viewer.x -= Z_DESP * sin(degToRad(viewer.yAngle));
		//viewer.z -= Z_DESP * cos(degToRad(viewer.yAngle));
		publishAction(ACTION_MOVE_UP);
	}else {
		cout << (int)key << endl;
	}
}*/

/*void keySpecialPressed(int key, int x, int y){
	switch(key) {
	case ROTATE_LEFT:
		//viewer.yAngle += 1.0;
		publishAction(ACTION_ROTATE_LEFT);
		break;
	case ROTATE_RIGHT:
		//viewer.yAngle -= 1.0;
		publishAction(ACTION_ROTATE_RIGHT);
		break;
	}
}*/



/*void initScreen(Screen scr){

	Monitor mon = scr.mon;
	glutInitWindowPosition(mon.x, mon.y);
	glutInitWindowSize(mon.w, mon.h);
	window = glutCreateWindow("Viewer Test 1");

	screens[glutGetWindow()] = scr;
	cout << "Asociado Window " << glutGetWindow() << " con Screen " << scr.id << endl;

	glutDisplayFunc([](){
		drawGLScene(glutGetWindow());
	});
	glutIdleFunc([](){
		drawGLScene(glutGetWindow());
	});
	glutReshapeFunc(&resizeGLScene);
	glutFullScreen();

	// Suscripcion de teclas (SERVER)
	glutKeyboardFunc(&keyPressed);
	glutSpecialFunc(&keySpecialPressed);

	InitGL();

	glViewport(0, 0, scr.mon.w, scr.mon.h);

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

}*/

/*int initServidor(){

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

}*/

void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;

	if(!strFileName)									// Return from the function if no file name was passed in
		return;

	pBitmap = auxDIBImageLoad(strFileName);				// Load the bitmap and store the data

	if(pBitmap == NULL)									// If we can't load the file, quit!
		exit(0);

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &textureArray[textureID]);

	// This sets the alignment requirements for the start of each pixel row in memory.
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR,
	// but looks blochy and pixilated.  Good for slower computers though.

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture

	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		}

		free(pBitmap);									// Free the bitmap structure
	}
}

void objTest(){
	CLoadObj g_LoadObj;										// This is OBJ class.  This should go in a good model class.
	t3DModel g_3DModel;

	g_LoadObj.ImportObj(&g_3DModel, FILE_NAME);			// Load our .Obj file into our model structure
	g_LoadObj.AddMaterial(&g_3DModel, "Bone", "Bone.bmp", 255, 255, 255); //
	g_LoadObj.SetObjectMaterial(&g_3DModel, 0, 0);
	for(int i = 0; i < g_3DModel.numOfMaterials; i++){
		// Check if the current material has a file name
		if(strlen(g_3DModel.pMaterials[i].strFile) > 0) {
			// Create a texture map from the material's file name
			CreateTexture(g_Texture, g_3DModel.pMaterials[i].strFile, i);
		}

		// Assign the material ID to the current material
		g_3DModel.pMaterials[i].texureId = i;
	}

	glEnable(GL_LIGHT0);								// Turn on a light with defaults set
	glEnable(GL_LIGHTING);								// Turn on lighting
	glEnable(GL_COLOR_MATERIAL);						// Allow color
}

typedef struct MyData {
    int val1;
    int val2;
} MYDATA, *PMYDATA;

void BounceProc( void *pMyID ){
	int    *MyID = (int*)pMyID;
	cout << "Soy un thread " << *MyID << endl;
	Sleep(5000);
	cout << "Termine " << endl;
}

int main(int argc, char **argv) {
	//int serv = initServidor();
	//PMYDATA data;
	//DWORD threadID;
	//HANDLE thread = CreateThread(NULL, 0, initServidor, data, 0, &threadID);

	//thread thr(initServidor);

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

	// Viewer (X, Y, Z)
	setViewer(0.0f, 0.0f, 5.0f);

	// Monitores
	setupMonitor();

	// Pantallas
	setupScreen();

	//initScreen(scr1);

	//initScreen(scr2);
	//initScreen(scr3);
	//initScreen(scr2, SCREEN_WIDTH, 0.0f, WIDTH, HEIGHT);
	// SCREEN_WIDTH, 0.0f, WIDTH, HEIGHT

	//cout << cos(degToRad(45)) << endl;
	//cout << cos(degToRad(15)) << endl;
	//cout << cos(degToRad(90)) << endl;

	initScreen(1, true);

	//initServer();

	// ------------------------ OBJ FILE ------------------------- //


	//objTest();

	glutMainLoop();

	return 1;
}

