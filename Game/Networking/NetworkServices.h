#pragma once
#include <winsock2.h>
#include <Windows.h>

//This is the port on the server (client port is randomly generated when connection is established)
#define DEFAULT_PORT "6881" 

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class NetworkServices
{

public:

	static int sendMessage(SOCKET curSocket, char* message, int messageSize);
	static int receiveMessage(SOCKET curSocket, char* buffer, int bufSize);

};