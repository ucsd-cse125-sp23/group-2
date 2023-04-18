#pragma once
#include "ServerNetwork.h"
#include <vector>
#include <queue>
#define TICK_RATE 128
#define DEBUG_BUFFER 100000
class ServerGame
{

public:

    queue<ClienttoServerData> incomingDataLists[NUM_CLIENTS];

    ServertoClientData gameState;

    ServerGame(void);
    ~ServerGame(void);

    void receiveFromClients();

    void packageData(ServertoClientData& data);

    void update();

    void step();

    void sendPackets();
private:

    // The ServerNetwork object 
    ServerNetwork* network;

    //Debug Buffer
    char debug[DEBUG_BUFFER];


};