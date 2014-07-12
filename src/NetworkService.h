/*
 * NetworkService.h
 *
 *  Created on: 11/07/2014
 *      Author: Hernan
 */

#ifndef NETWORKSERVICE_H_
#define NETWORKSERVICE_H_

#pragma once
#include <winsock2.h>
#include <windows.h>

class NetworkService {
public:
	static int sendMessage(SOCKET curSocket, char * message, int messageSize){
		return send(curSocket, message, messageSize, 0);
	}
	static int receiveMessage(SOCKET curSocket, char * buffer, int bufSize){
		return recv(curSocket, buffer, bufSize, 0);
	}
};

#endif /* NETWORKSERVICE_H_ */
