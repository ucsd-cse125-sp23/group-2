#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "NetworkData.h"
#include "graphics/GameWindow.h"

class ClientGame
{
private:
    GameWindow* gameWindow;
    static bool moveForward, moveBack, moveLeft, moveRight;
    static bool playerattacking;
public:

    ServertoClientData incomingData;
    ServertoClientInit initData;
    ClientGame();
    ~ClientGame(void);

    ClientNetwork* network;

    void packageData(ClienttoServerData & data);

    int recieveData();

    void update();

    void setup_callbacks();

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};
