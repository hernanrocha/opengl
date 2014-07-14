/*
 * Client.h
 *
 *  Created on: 10/07/2014
 *      Author: Hernan
 */

#ifndef CLIENT_H_
#define CLIENT_H_

//#include "Graphics.h"
#include <iostream>
#include <math.h>
#include <map>
#include <stdio.h>
#include <unistd.h>     // Header file for sleeping.
//#include <windows.h>
#include <process.h>
//#include <conio.h>
#include <GL/glut.h>
#include <GL/GLAux.h>

// Teclado
#define ESCAPE 27
#define MOVE_LEFT 'a'
#define MOVE_RIGHT 'd'
#define MOVE_UP 'w'
#define MOVE_DOWN 's'
#define ROTATE_LEFT GLUT_KEY_LEFT
#define ROTATE_RIGHT GLUT_KEY_RIGHT

// Acciones internas
#define ACTION_MOVE_LEFT 1
#define ACTION_MOVE_RIGHT 2
#define ACTION_MOVE_UP 3
#define ACTION_MOVE_DOWN 4
#define ACTION_ROTATE_LEFT 5
#define ACTION_ROTATE_RIGHT 6

// Server
#define MAXBUFLEN 512
//#define SERVER_IP "192.168.1.35"
#define SERVER_PORT 9999

// Client
#define CLIENT_PORT 10000

// Desplazamiento
#define X_DESP 0.5
#define Z_DESP 0.5

// Constantes
const float piover180 = 0.0174532925f;

struct Viewer {
	GLfloat x, y, z;
	GLfloat yAngle;
};

struct Monitor {
	int id;
	int x, y, w, h;
};

struct Screen {
	int id;
	GLdouble centerX, centerY, centerZ;
	GLfloat left, right, bottom, top, dist;
	Monitor mon;
};

#include <winsock.h>    // Soporte a Sockets

// Enet
#include "enet/enet.h"

using namespace std;

//class Client {
//public:
//	Client();
//	virtual ~Client();


void setViewer(GLfloat x, GLfloat y, GLfloat z);
void setupMonitor();
void setupScreen();
void setServer(char * ip, u_short port);

void addMonitor(Monitor mon);
void initScreen(int screenID, bool server = true);
void drawGLScene(int windowID);

// Server
void initServer();
void publishAction(int action);
void keyPressed(unsigned char key, int x, int y);
void keySpecialPressed(int key, int x, int y);

// Client
void initClient();
void processAction(int action);

// Test Models
void drawSnowMan();
void snowTest();

// Enet
void initEnetServer();
void initEnetClient();


//private:
//};

#endif /* CLIENT_H_ */
