#include "ClientGame.h"

bool ClientGame::moveForward = 0;
bool ClientGame::moveBack = 0;
bool ClientGame::moveRight = 0;
bool ClientGame::moveLeft = 0;
bool ClientGame::playerattacking = 0;
bool ClientGame::jumping = 0;
int ClientGame::build = 0;

ClientGame::ClientGame(void)
{


    //Network Initializatio
    network = new ClientNetwork();
    network->initConnection();

    //TODO Game Initialization
    gameWindow = new GameWindow(800, 600);
    setup_callbacks();

}


//Converts from network's data to gamedata
int ClientGame::recieveData()
{
    network->recieveDeserialize(incomingData, initData);
    return 0;
}

void ClientGame::update()
{
    static int count = 0;
    count++;
    //TODO Render

    // Draw nothing, see you in tutorial 2 !

    //Recieve Data
    //Recieve incoming server data into gamestate
    recieveData();

    /*
    for (int i = 0; i < incomingData.logsize; ++i) {
        printf("Recieved %d combat logs: Ent %d, attacked Ent %d, for dmg %f\n", incomingData.logsize, incomingData.combatLogs[i].source, incomingData.combatLogs[i].target, incomingData.combatLogs[i].damage);
        if (incomingData.combatLogs[i].killed) {
            printf("And killed it\n");
        }
    }
    */
    //Send Data to Server
    ClienttoServerData newPackage;
    packageData(newPackage);
    //std::cout << newPackage.moveForward << "\n";
    network->sendActionPackets(newPackage);

    //pass through ServertoClientData
    //Check init connection
    gameWindow->update(incomingData, initData.id);
    
    
}

void ClientGame::packageData(ClienttoServerData& data) {
    data.moveForward = moveForward;
    data.moveBack = moveBack;
    data.moveLeft = moveLeft;
    data.moveRight = moveRight;
    data.shoot = playerattacking;
    data.jump = jumping;
    data.build = build;
    data.camAngleAroundPlayer = gameWindow->getCamAngle();
    data.camDirectionVector = gameWindow->getCamDirectionVector();
    data.camPosition = gameWindow->getCamPosition();
}

void ClientGame::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_W:
            moveForward = true;
            break;
        case GLFW_KEY_A:
            moveLeft = true;
            break;
        case GLFW_KEY_S:
            moveBack = true;
            break;
        case GLFW_KEY_D:
            moveRight = true;
            break;
        case GLFW_KEY_SPACE:
            jumping = true;
            break;
        case GLFW_KEY_E:
            if (build != 0) {
                build = 0;
            }
            else {
                build = 1;
            }
            break;
        default: break;
        }
    }
    else if (action == GLFW_RELEASE) {
        switch (key) {
        case GLFW_KEY_W:
            moveForward = false;
            break;
        case GLFW_KEY_A:
            moveLeft = false;
            break;
        case GLFW_KEY_S:
            moveBack = false;
            break;
        case GLFW_KEY_D:
            moveRight = false;
            break;
        case GLFW_KEY_SPACE:
            jumping = false;
            break;
        default: break;
        }
    }
}

void ClientGame::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            playerattacking = true;
            break;
        default: break;
        }
    }
    else if (action == GLFW_RELEASE) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            playerattacking = false;
            break;
        default: break;
        }
    }
}

void ClientGame::setup_callbacks() {
    // Set the key callback.
    glfwSetKeyCallback(gameWindow->window, ClientGame::keyCallback);

    glfwSetCursorPosCallback(gameWindow->window, GameWorld::cursor_callback);

    // Set the window resize callback.
    glfwSetWindowSizeCallback(gameWindow->window, GameWindow::resizeCallback);
    glfwSetMouseButtonCallback(gameWindow->window, ClientGame::mouse_button_callback);
}
