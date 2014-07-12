/*
 * ClientNetwork.h
 *
 *  Created on: 11/07/2014
 *      Author: Hernan
 */

#ifndef CLIENTNETWORK_H_
#define CLIENTNETWORK_H_

// Networking libraries
#include <winsock2.h>
#include <windows.h>
#include "NetworkService.h"
#include <ws2tcpip.h>
#include <stdio.h>

// size of our buffer
#define DEFAULT_BUFLEN 512
// port to connect sockets through
#define DEFAULT_PORT "6881"

class ClientNetwork {
public:
	// for error checking function calls in Winsock library
	int iResult;

	// socket for client to connect to server
	SOCKET ConnectSocket;


	ClientNetwork();
	virtual ~ClientNetwork();
};

#endif /* CLIENTNETWORK_H_ */
