#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
class ClientGame
{

public:
    char network_data[MAX_PACKET_SIZE];


    ClientGame();
    ~ClientGame(void);

    ClientNetwork* network;

    void sendActionPackets();



    void update();
};
