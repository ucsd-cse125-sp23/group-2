#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"

class ClientGame
{

public:

    ServertoClientData incomingData;
    ClientGame();
    ~ClientGame(void);

    ClientNetwork* network;

    void sendActionPackets();

    int recieveData();

    void update();
};
