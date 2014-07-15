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

void printIP(enet_uint32 host){
	int a,b,c,d;
	a = host % 256;
	host = (host - a) / 256;
	b = host % 256;
	host = (host - b) / 256;
	c = host % 256;
	host = (host - c) / 256;
	d = host % 256;
	cout << a << "." << b << "." << c << "." << d;
}

Viewer viewer;
map<int, Monitor> monitors;
map<int, Screen> screens;
map<int, Screen> windows;

// Servidor
char RecvBuff[MAXBUFLEN];
SOCKET comm_socket;
char * serverIP;
//u_short serverPort;

// Cliente
SOCKET sock;

// ENET
ENetHost * server;
ENetPeer * serverPeer;
ENetHost * client;
ENetPeer * clientPeer;

void enviarHello(){
	cout << "[Server] Saludo enviado" << endl;
	ENetPacket * packet = enet_packet_create ("iasdasd",	strlen ("HolaSoyServer") + 1, ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(serverPeer, 0, packet);
	enet_host_flush(server);
}

void enviarHelloCliente(){
	/* Create a reliable packet of size 7 containing "packet\0" */
	ENetPacket * packet = enet_packet_create ("HolaSoyCliente",	strlen ("HolaSoyCliente") + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(clientPeer, 0, packet);

	enet_host_flush (client);
	cout << "[Client] Saludo enviado" << endl;
}

void initPollingCliente(void *pMyID){
	ENetEvent event;
	unsigned char * dato;
	int action;

	// Wait up to 1000 milliseconds for an event.
	while (enet_host_service(client, & event, 25000) > 0)
	{
		switch (event.type){
		case ENET_EVENT_TYPE_CONNECT:
			cout << "[Client] A new connection from " << event.peer->address.host << ":" << event.peer->address.port << endl;
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			cout << "[Client] Paquete recibido." << endl;
			//cout << "[Client] " << event.packet->data << endl;

			dato = event.packet->data;
			action = atoi((char *) dato);
			cout << "[Client] Accion recibida " << action << endl;
			processAction(action);

			//event.packet -> dataLength,
			//event.packet -> data,
			//event.peer -> data,
			//event.channelID);
			// Clean up the packet now that we're done using it.
			enet_packet_destroy (event.packet);

			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			printf ("%s disconnected.\n", event.peer -> data);
			// Reset the peer's client information.
			event.peer -> data = NULL;
			break;
		default:
			cout << "Type: " << event.type << endl;
		}

	}
}

void initPolling(void *pMyID){
	ENetEvent serverEvent;

	// Wait up to 1000 milliseconds for an event.
	while (enet_host_service(server, & serverEvent, 25000) > 0)
	{
	    switch (serverEvent.type){
	    case ENET_EVENT_TYPE_CONNECT:
	        cout << "[Server] A new connection from " << serverEvent.peer->address.host << endl;
	        serverPeer = serverEvent.peer;
	        // Store any relevant client information here.
	        //event.peer -> data = "Client information";
	        enviarHello();
	        break;
	    case ENET_EVENT_TYPE_RECEIVE:
	        cout << "[Server] Packet of length " << serverEvent.packet->dataLength << " from " << serverEvent.peer->address.host << endl;
	        cout << "[Server] " << serverEvent.packet->data << endl;
	                //event.packet -> dataLength,
	                //event.packet -> data,
	                //event.peer -> data,
	                //event.channelID);
	        // Clean up the packet now that we're done using it.
	        enet_packet_destroy (serverEvent.packet);

	        break;

	    case ENET_EVENT_TYPE_DISCONNECT:
	        printf ("%s disconnected.\n", serverEvent.peer -> data);
	        // Reset the peer's client information.
	        serverEvent.peer -> data = NULL;
	        break;
	    default:
	    	cout << "Type: " << serverEvent.type << endl;
	    }

	}
}

void initEnetServer(){
	// Server Address
	ENetAddress serverAddress;
	enet_address_set_host(& serverAddress, "");

	//serverAddress.host = ENET_HOST_ANY;
	serverAddress.port = SERVER_PORT;

	// Create Host
	server = enet_host_create (& serverAddress /* the address to bind the server host to */,
			32      /* allow up to 32 clients and/or outgoing connections */,
			2      /* allow up to 2 channels to be used, 0 and 1 */,
			0      /* assume any amount of incoming bandwidth */,
			0      /* assume any amount of outgoing bandwidth */);

	if (server == NULL){
		cout << "An error occurred while trying to create an ENet server host." << endl;
		exit (EXIT_FAILURE);
	} else {
		cout << "Servidor abierto en ";
		printIP(serverAddress.host);
		cout << ":" << serverAddress.port << endl;
		int threadID = 10;
		_beginthread(initPolling, 0, &threadID);
	}
}

void initEnetClient(){
	ENetAddress address;
	enet_address_set_host (&address, serverIP);
	address.port = SERVER_PORT;

	// Crear Cliente
	client = enet_host_create (NULL /* create a client host */,
			1 /* only allow 1 outgoing connection */,
			2 /* allow up 2 channels to be used, 0 and 1 */,
			57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
			14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

	if (client == NULL){
		fprintf (stderr,
				"An error occurred while trying to create an ENet client host.\n");
		exit (EXIT_FAILURE);
	}else{
		cout << "[Client] Host abierto " << endl;
	}

	// Crear conexion
	ENetPeer *peer;
	peer = enet_host_connect (client, &address, 2, 0);
	if (peer == NULL){
		cout << "No available peers for initiating an ENet connection." << endl;
		exit (EXIT_FAILURE);
	}else{
		cout << "[Client] Conexion preparada" << endl;
	}

	ENetEvent eventCliCon;
	if (enet_host_service (client, & eventCliCon, 5000) > 0 &&	eventCliCon.type == ENET_EVENT_TYPE_CONNECT)
	{
	    cout << "[Client] Connection succeeded." << endl;

	    // Init Polling
		int threadID = 11;
		_beginthread(initPollingCliente, 0, &threadID);

		// Enviar Hello desde Cliente
		clientPeer = peer;
		enviarHelloCliente();
	} else {
	    enet_peer_reset (peer);
	    puts ("[Client] Connection failed.");
	}
}

void setServer(char * ip){
	cout << "Set Server " << ip << endl;
	serverIP = ip;
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
		cout << "Caracteres no identificados" << endl;
		return;
	}

	// Actualizar
	glutPostRedisplay();
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

	// Monitor 3
	Monitor lg47;
	lg47.id = 3;
	lg47.x = 0;
	lg47.y = 0;
	lg47.w = 1920;
	lg47.h = 1080;
	addMonitor(lg47);

	Monitor lg47b;
	lg47b.id = 4;
	lg47b.x = 1366;
	lg47b.y = 0;
	lg47b.w = 1920;
	lg47b.h = 1080;
	addMonitor(lg47b);

}

void setupScreen(){
	// Screen 1 (Pantalla Izquierda)
	// Centro: (0, 0, -5)
	// Dimensiones (en dm): (-3.45; 0) x (-0.975; 0.975)
	Screen scr1;
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

void loadINI(){
	INIReader reader("test.ini");

	if (reader.ParseError() < 0) {
		cout << "Can't load 'test.ini'\n";
		return;
	}

	int cantScreens = reader.GetInteger("screens", "cant", 0);
	cout << "Hay " << cantScreens << " screens" << endl;

	for (int i = 1; i <= cantScreens; i++){
		char section[16];
		sprintf(section, "scr%d", i);

		Screen scr;
		scr.id = reader.GetInteger(section, "id", 0);
		scr.centerX = reader.GetReal(section, "centerX", 0.0f);
		scr.centerY = reader.GetReal(section, "centerY", 0.0f);
		scr.centerZ = reader.GetReal(section, "centerZ", 0.0f);
		scr.left = reader.GetReal(section, "left", 0.0f);
		scr.right = reader.GetReal(section, "right", 0.0f);
		scr.bottom = reader.GetReal(section, "bottom", 0.0f);
		scr.top = reader.GetReal(section, "top", 0.0f);
		scr.dist = reader.GetReal(section, "dist", 0.0f);
		scr.mon = monitors[reader.GetInteger(section, "mon", 0)];

		cout << "Add Screen " << scr.id << endl;
		screens[scr.id] = scr;
	}


	/*cout << "Config loaded from 'test.ini': version="
			<< reader.GetInteger(screen, "version", -1) << ", name="
			<< reader.Get("user", "name", "UNKNOWN") << ", email="
			<< reader.Get("user", "email", "UNKNOWN") << ", pi="
			<< reader.GetReal("user", "pi", -1) << ", active="
			<< reader.GetBoolean("user", "active", true) << "\n";*/
	//string nombreScreen = reader.Get("screen" + 1, )
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
	char bufferSalida[1];
	sprintf(bufferSalida,"%d",action);
	processAction(action);
	//send(comm_socket,bufferSalida,strlen(bufferSalida),0);
	//send(comm_socket,bufferSalida,strlen(bufferSalida), MSG_OOB);
	ENetPacket * packet = enet_packet_create (bufferSalida,	strlen (bufferSalida) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server, 0, packet);
	enet_host_flush(server);

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
