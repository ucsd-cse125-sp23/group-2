#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "graphics/GameWindow.h"

class ClientGame
{
private:
    GameWindow* gameWindow;
public:

    ServertoClientData incomingData;
    ClientGame();
    ~ClientGame(void);

    ClientNetwork* network;

    void sendActionPackets();

    int recieveData();

    void update();

    void setup_callbacks();
};
