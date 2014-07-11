/*
 * Client.cpp
 *
 *  Created on: 10/07/2014
 *      Author: Hernan
 */

#include "Client.h"

float degToRad(float deg){
	return deg * piover180;
}

Viewer viewer;
map<int, Monitor> monitors;
map<int, Screen> screens;
map<int, Screen> windows;

// Servidor
char RecvBuff[MAXBUFLEN];

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
	port = htons(SERVER_PORT);

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
	server.sin_port = htons(SERVER_PORT);
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

void initServer(){
	int threadID = 10;
	cout << "[initServer] Iniciando Thread Server" << endl;
	//_beginthread(initServidor, 0, &threadID);
	cout << "[initServer] Iniciado" << endl;
}

void processAction(int action){
	switch (action) {
	case ACTION_MOVE_LEFT:
		viewer.x -= X_DESP * cos(degToRad(viewer.yAngle));
		viewer.z += X_DESP * sin(degToRad(viewer.yAngle));
		break;
	case ACTION_MOVE_RIGHT:
		viewer.x += X_DESP * cos(degToRad(viewer.yAngle));
		viewer.z -= X_DESP * sin(degToRad(viewer.yAngle));
		break;
	case ACTION_MOVE_UP:
		viewer.x -= Z_DESP * sin(degToRad(viewer.yAngle));
		viewer.z -= Z_DESP * cos(degToRad(viewer.yAngle));
		break;
	case ACTION_MOVE_DOWN:
		viewer.x += Z_DESP * sin(degToRad(viewer.yAngle));
		viewer.z += Z_DESP * cos(degToRad(viewer.yAngle));
		break;
	case ACTION_ROTATE_LEFT:
		viewer.yAngle += 1.0;
		break;
	case ACTION_ROTATE_RIGHT:
		viewer.yAngle -= 1.0;
		break;
	default:
		break;
	}
}

void setViewer(GLfloat x, GLfloat y, GLfloat z){
	cout << "Set Viewer" << endl;
	viewer.x = x;
	viewer.y = y;
	viewer.z = z;
}

void addMonitor(Monitor mon){
	cout << "Add Monitor " << mon.id << endl;
	monitors[mon.id] = mon;
}

void setupMonitor(){
	// Monitor 1
	Monitor mon1;
	mon1.id = 1;
	mon1.x = 0;
	mon1.y = 0;
	mon1.w = 1366;
	mon1.h = 768;
	addMonitor(mon1);

	// Monitor 2
	Monitor mon2;
	mon2.id = 2;
	mon2.x = 1366;
	mon2.y = 0;
	mon2.w = 1440;
	mon2.h = 900;
	addMonitor(mon2);
}

void setupScreen(){
	// Screen 1 (Pantalla Izquierda)
	// Centro: (0, 0, -5)
	// Dimensiones (en dm): (-3.45; 0) x (-0.975; 0.975)
	Screen scr1;
	cout << "salida " << endl;
	scr1.id = 1;
	scr1.centerX = 0.0f;
	scr1.centerY = 0.0f;
	scr1.centerZ = -5.0f;
	scr1.left = -3.45f;
	scr1.right = 0.0f;
	scr1.bottom = -0.975f;
	scr1.top = 0.975f;
	scr1.dist = 5.0f;
	scr1.mon = monitors[1];

	// Screen 2 (Pantalla Derecha)
	// Centro: (0, 0, -5)
	// Dimensiones (en dm): (0; 3.7) x (-0.275; 2.025) [0.7 mas arriba que el otro]
	Screen scr2;
	scr2.id = 2;
	scr2.centerX = 0.0f;
	scr2.centerY = 0.0f;
	scr2.centerZ = -5.0f;
	scr2.left = 0.0f;
	scr2.right = 3.7f;
	scr2.bottom = -0.275f;
	scr2.top = 2.025f;
	scr2.dist = 5.0f;
	scr2.mon = monitors[2];

	// Screen 3 (Pantalla Perpendicular)
	Screen scr3;
	scr3.id = 3;
	scr3.centerX = -3.45f;
	scr3.centerY = 0.0f;
	scr3.centerZ = 0.0f;
	scr3.left = 1.3f;
	scr3.right = 5.0f;
	scr3.bottom = -0.275f;
	scr3.top = 2.025f;
	scr3.dist = 3.45f;
	scr3.mon = monitors[2];

	// Screen 4 (Pantalla Frontal)
	Screen scr4;
	scr4.id = 4;
	scr4.centerX = 0.0f;
	scr4.centerY = 0.0f;
	scr4.centerZ = -5.0f;
	scr4.left = -1.725f;
	scr4.right = 1.725f;
	scr4.bottom = -0.975f;
	scr4.top = 0.975f;
	scr4.dist = 5.0f;
	scr4.mon = monitors[1];

	cout << "Add Screen " << endl;
	screens[1] = scr1;
	screens[2] = scr2;
	screens[3] = scr3;
	screens[4] = scr4;
}

void drawSnowMan() {

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
}

void drawGLScene(int windowID){
	Screen scr = windows[windowID];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(-viewer.yAngle, 0.0f, 1.0f, 0.0f);

	gluLookAt(viewer.x, viewer.y + 1, viewer.z,
			(viewer.x + scr.centerX), (viewer.y + 1 + scr.centerY), (viewer.z + scr.centerZ),
			0.0f, 1.0f, 0.0f);

	snowTest();

	glutSwapBuffers();
	glutPostRedisplay();
}

void publishAction(int action){
	processAction(action);

}

void keyPressed(unsigned char key, int x, int y){
	// avoid thrashing this procedure
	usleep(100);

	// If escape is pressed, kill everything.
	if (key == ESCAPE){
		// shut down our window
		glutDestroyWindow(glutGetWindow());
		exit(0);
	}else if (key == 'l'){
		//light=!light;               // Toggle Light TRUE/FALSE
		//if (!light)             // If Not Light
		//{
		//	glDisable(GL_LIGHTING);     // Disable Lighting
		//}
		//else                    // Otherwise
		//{
		//	glEnable(GL_LIGHTING);      // Enable Lighting
		//}
	}else if (key == 'm'){
		//move = !move;
	}else if (key == 't'){
		//desplazar = !desplazar;
		//desplazarMirror = desplazar;
	}else if (key == MOVE_RIGHT){
		publishAction(ACTION_MOVE_RIGHT);
	}else if (key == MOVE_LEFT){
		publishAction(ACTION_MOVE_LEFT);
	}else if (key == MOVE_DOWN){
		publishAction(ACTION_MOVE_DOWN);
	}else if (key == MOVE_UP){
		publishAction(ACTION_MOVE_UP);
	}else {
		cout << (int)key << endl;
	}
}

void keySpecialPressed(int key, int x, int y){
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
}

void initGL(){
	// Jump To Texture Loading Routine
	//if (!loadGLTextures()){
	//	cout << "Textura no cargada " << endl;			// Texture Didn't Load
	//}

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping

	// Iluminacion:
	//    * GL_FLAT (Color plano)
	//    * GL_SMOOTH (Interpolacion de color entre los 3 vertices)
	//    * PHONG (Interpolacion de normales de los 3 vertices y posterior calculo de color)
	// Ademas, definir normal para cada vertice / triangulo
	glShadeModel(GL_SMOOTH);

	// Material:

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Luces
	//glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	//glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	//glEnable(GL_LIGHT1);


	// Culling
	//glCullFace(GL_FRONT);
	//glEnable(GL_CULL_FACE);
}

void initScreen(int screenID, bool server){
	Screen scr = screens[screenID];
	Monitor mon = scr.mon;
	glutInitWindowPosition(mon.x, mon.y);
	glutInitWindowSize(mon.w, mon.h);
	glutCreateWindow("Viewer Test");

	windows[glutGetWindow()] = scr;
	cout << "Asociado Window " << glutGetWindow() << " con Screen " << scr.id << endl;

	glutDisplayFunc([](){
		drawGLScene(glutGetWindow());
	});
	glutIdleFunc([](){
		drawGLScene(glutGetWindow());
	});

	//glutReshapeFunc(&resizeGLScene);
	glutFullScreen();

	// Suscripcion de teclas (SERVER)
	if (server){
		glutKeyboardFunc(&keyPressed);
		glutSpecialFunc(&keySpecialPressed);
	}

	initGL();

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

}
