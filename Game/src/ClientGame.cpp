#include "ClientGame.h"

bool ClientGame::moveForward = 0;
bool ClientGame::moveBack = 0;
bool ClientGame::moveRight = 0;
bool ClientGame::moveLeft = 0;
bool ClientGame::playerattacking = 0;
bool ClientGame::jumping = 0;
int ClientGame::build = 0;
int ClientGame::selected = 0;
bool ClientGame::upgrade = 0;

ClientGame::ClientGame(void)
{
    //TODO Game Initialization
    gameWindow = new GameWindow(RES_WIDTH, RES_HEIGHT);
    setup_callbacks();
    audioManager = new AudioManager();

    initData.id = INVALID_CLIENT_ID;
    incomingData.serverStatus = UNKNOWN_SERVER_STATUS;

    //Network Initialization
    network = new ClientNetwork();
    network->initConnection();
    
}


//Converts from network's data to gamedata
int ClientGame::recieveData()
{
    return network->recieveDeserialize(incomingData, initData);
}

void ClientGame::update()
{
    //Recieve Data
    //Recieve incoming server data into gamestate
    recieveData();

    //Send Data to Server
    ClienttoServerData newPackage;
    packageData(newPackage);
    network->sendActionPackets(newPackage);

    //Render
    if (initData.id != INVALID_CLIENT_ID && incomingData.serverStatus != UNKNOWN_SERVER_STATUS) {
        gameWindow->update(incomingData, initData.id);
        audioManager->update(gameWindow->getCamPosition(), glm::normalize(gameWindow->getCamDirectionVector()), glm::normalize(gameWindow->getCamUpVector()), incomingData);
    }
    
    //Process combat logs
    for (int i = 0; i < incomingData.clogsize; ++i) {
        Entity target = incomingData.combatLogs[i].target;
        //printf("Recieved %d combat logs: Ent %d, attacked Ent %d, for dmg %f\n", incomingData.logsize, incomingData.combatLogs[i].source, incomingData.combatLogs[i].target, incomingData.combatLogs[i].damage);
        if (incomingData.combatLogs[i].killed) {
            //printf("And killed it\n");
        }
    }
    incomingData.clogsize = 0;

    //Process sound logs
    for (int i = 0; i < incomingData.slogsize; ++i) {
        Entity source = incomingData.soundLogs[i].source;
        if (!incomingData.soundLogs[i].stop) {
            glm::vec3 position = incomingData.positions[source];
            audioManager->playSound(incomingData.models[source].modelID, incomingData.soundLogs[i].sound, position, source);
        }
        else {
            audioManager->stopSound(source);
        }
    }
    incomingData.slogsize = 0;

}

void ClientGame::packageData(ClienttoServerData& data) {
    data.moveForward = moveForward;
    data.moveBack = moveBack;
    data.moveLeft = moveLeft;
    data.moveRight = moveRight;
    data.shoot = playerattacking;
    data.jump = jumping;
    data.build = build;
    data.selected = selected;
    data.camAngleAroundPlayer = gameWindow->getCamAngle();
    data.camDirectionVector = gameWindow->getCamDirectionVector();
    data.camPosition = gameWindow->getCamPosition();
    data.upgrade = upgrade;
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
                upgrade = 0;
            }
            else {
                upgrade = 0;
                build = 1;
            }
            selected = 0;
            break;
        case GLFW_KEY_R:
            if (upgrade != 0) {
                build = 0;
                upgrade = 0;
            }
            else {
                build = 0;
                upgrade = 1;
            }
            break;
        case GLFW_KEY_1:
            selected = 0;
            break;
        case GLFW_KEY_2:
            selected = 1;
            break;
        case GLFW_KEY_3:
            selected = 2;
            break;
        case GLFW_KEY_4:
            selected = 3;
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
    //glfwSetMouseButtonCallback(gameWindow->window, GameWorld::mouse_button_callback);

}
