#pragma once
#include "ServerNetwork.h"
#include <vector>

class ServerGame
{

public:

    vector<ClienttoServerData> incomingDataList;

    ServerGame(void);
    ~ServerGame(void);

    void receiveFromClients();

    void update();
private:

    // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

    // The ServerNetwork object 
    ServerNetwork* network;

};