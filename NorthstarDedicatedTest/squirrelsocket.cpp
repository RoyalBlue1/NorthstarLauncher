#include "pch.h"
#include "squirrelsocket.h"
#include <WinSock2.h>


#pragma comment(lib,"ws2_32.lib")

SOCKET clientSock = INVALID_SOCKET;

char* queue[64];
int queueReadPointer;
int queueWritePointer;

int initSquirrelSocket(int port);
int sendSquirrelSocket(char* data, int dataLength);
SQRESULT SQ_SendSocketMessage(void* sqvm);
SQRESULT SQ_ReadSocketMessage(void* sqvm);
SQRESULT SQ_initSocket(void* sqvm);

int initSquirrelSocket(int port) {
	//open tcp socket
	WSADATA wsaData;
	SOCKET sock;
	struct sockaddr_in server,client;
	queueReadPointer = 0;
	queueWritePointer = 0;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		spdlog::error("Failed. Error Code : {}", WSAGetLastError());
		return -1;
	}
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		spdlog::error("Could not create socket : {}", WSAGetLastError());
		return -1;
	}
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (bind(sock,(struct sockaddr *) & server, sizeof(server)) == SOCKET_ERROR) {
		spdlog::error("Bind failed with error code : {}", WSAGetLastError());
		return -1;
	}

	while (true) {
		listen(sock, 3);
		int c = sizeof(struct sockaddr_in);
		clientSock = accept(sock, (struct sockaddr*)&client, &c);
		if (clientSock == INVALID_SOCKET) {
			spdlog::error("Accept failed with error code : {}", WSAGetLastError());
			return -1;
		}
		while (true) {
			char data[2000];
			int dataLength = recv(clientSock, data, 2000, 0);
			if (dataLength == SOCKET_ERROR) {
				spdlog::error("Recv failed with error code : {}", WSAGetLastError());
				return -1;
			}
			char* storedData = (char*) malloc(dataLength + 1);
			memcpy(storedData, data, dataLength);
			storedData[dataLength] = '\0';
			queue[queueWritePointer] = storedData;
			queueWritePointer = (queueWritePointer + 1) % 64;
		}

	}
	
	
	return 0;
	
}

int sendSquirrelSocket(const char* data, int dataLength) {
	if (clientSock != INVALID_SOCKET) {
		int n = send(clientSock, data, dataLength,0);
		if (n < 0) {
			spdlog::error("ERROR writing to socket");
			return -1;
		}
		return 0;
	}
	spdlog::warn("socket not initialized");
}




SQRESULT SQ_ReadSocketMessage(void* sqvm)
{	
	if (queueReadPointer != queueWritePointer) {
		char* message = queue[queueReadPointer];
		queue[queueReadPointer] = 0;
		if ((long long)message == 0) {
			return SQRESULT_ERROR;
		}
		queueReadPointer = (queueReadPointer + 1) % 64;
		ServerSq_pushstring(sqvm, message, -1);
		free(message);
		
	}	
	else 
	{
		ServerSq_pushstring(sqvm, "", -1);
	}
		
	return SQRESULT_NOTNULL;
}

SQRESULT SQ_SendSocketMessage(void* sqvm)
{
	const char* message = ServerSq_getstring(sqvm, 1);
	int messageLength = strlen(message);
	sendSquirrelSocket(message, messageLength);
	return SQRESULT_NULL;
}

SQRESULT SQ_initSocket(void* sqvm)
{
	std::thread socketThread(initSquirrelSocket, ServerSq_getinteger(sqvm, 1));
	socketThread.detach();
	return SQRESULT_NULL;
}

void InitialiseSquirrelSocket(HMODULE baseAddress) {
	
	g_ServerSquirrelManager->AddFuncRegistration("void", "NSInitSocket", "int port", "", SQ_initSocket);
	g_ServerSquirrelManager->AddFuncRegistration("void","NSSendSocketMessage","string message","",SQ_SendSocketMessage);
	g_ServerSquirrelManager->AddFuncRegistration("string", "NSReadSocketMessage", "", "", SQ_ReadSocketMessage);

}