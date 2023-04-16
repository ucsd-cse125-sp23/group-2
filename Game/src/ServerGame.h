#pragma once
#include "ServerNetwork.h"
#include <vector>
#include <queue>
#define TICK_RATE 128

class ServerGame
{

public:

    queue<ClienttoServerData> incomingDataList;

    ServertoClientData gameState;

    ServerGame(void);
    ~ServerGame(void);

    void receiveFromClients();

    void packageData(ServertoClientData& data);

    void update();

    void step();
private:

    // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

    // The ServerNetwork object 
    ServerNetwork* network;

};