#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "NetworkServices.h"
#include <ws2tcpip.h>
#include <map>
#include "NetworkData.h"
#include<vector>
#include<queue>
using namespace std;
#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512

class ServerNetwork
{
private:
    // receive incoming packets from clients
    int receivePackets(unsigned int client_id, char* recvbuf);
    // send packet to all clients
    void sendToAll(char* packets, int totalSize);

public:

    ServerNetwork(void);
    ~ServerNetwork(void);

    // Socket to listen for new connections
    SOCKET ListenSocket;

    // Socket to give to the clients
    SOCKET ClientSocket;

    // for error checking return values
    int iResult;

    // table to keep track of each client's socket
    std::map<unsigned int, SOCKET> sessions;


    // data buffer
    char network_data[MAX_PACKET_SIZE];

    // accept new connections
    bool acceptNewClient(unsigned int& id);

    // receive incoming data
    int receiveDeserialize(queue<ClienttoServerData>& incomingDataList);
    
    // send data to all clients
    void sendActionPackets(ServertoClientData & outgoingData);
};