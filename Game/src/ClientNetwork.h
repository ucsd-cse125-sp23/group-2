#pragma once
// Networking libraries
#include <winsock2.h>
#include <Windows.h>
#include "NetworkServices.h"
#include <ws2tcpip.h>
#include <stdio.h> 
#include "NetworkData.h"

// size of our buffer
#define DEFAULT_BUFLEN 512
// Server IP
#define SERVER_IP "127.0.0.1"
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class ClientNetwork
{
private: 
    // receive incoming packets from server
    int receivePackets(char*);
public:

    unsigned int ndloc;
    char network_data[MAX_PACKET_SIZE];

    //OutgoingDataServer network_data;

    // for error checking function calls in Winsock library
    int iResult;

    // socket for client to connect to server
    SOCKET ConnectSocket;

    // ctor/dtor
    ClientNetwork(void);
    ~ClientNetwork(void);

    int initConnection();

    // receive incoming data
    int recieveDeserialize(ServertoClientData & incomingData, ServertoClientInit& initData);

    // send data to all server
    void sendActionPackets(ClienttoServerData & outgoingData);
};