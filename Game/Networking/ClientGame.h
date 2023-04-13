#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ClientGame
{

public:

    ServertoClientData incomingData;
    GLFWwindow* window;

    ClientGame();
    ~ClientGame(void);

    ClientNetwork* network;

    void sendActionPackets();

    int recieveData();

    void update();
};
