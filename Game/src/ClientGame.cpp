#include "ClientGame.h"

bool ClientGame::moveForward = 0;
bool ClientGame::moveBack = 0;
bool ClientGame::moveRight = 0;
bool ClientGame::moveLeft = 0;
bool ClientGame::playerattacking = 0;
extern int selected;
extern	std::array <GUIElement*, NUM_GUI> guis;
extern int depth;
extern bool menuOn;
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
    data.camAngleAroundPlayer = gameWindow->getCamAngle();
    data.camDirectionVector = gameWindow->getCamDirectionVector();
    data.camPosition = gameWindow->getCamPosition();
}


void handle_down() {
    std::cout << "depth"<< depth << std::endl;
    switch (depth) {
    case 0:
        switch (selected) {
        case 0:
            guis[0]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/continue.jpg");
            break;
        case 1:
            guis[1]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/options.jpg");
            break;
        case 2:
            guis[2]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/quit.jpg");
            break;
        }
        selected++;
        if (selected > 2) {
            selected = 0;
        }
        switch (selected) {
        case 0:
            guis[0]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/continueH.jpg");
            break;
        case 1:
            guis[1]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/optionsH.jpg");
            break;
        case 2:
            guis[2]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/quitH.jpg");
            break;
        }
    break;
    case 1:
        switch (selected) {
            case 3:
                guis[3]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/back.jpg");
                break;
            case 4:
                guis[4]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/volume.jpg");
                break;
        }
        selected++;
        std::cout << "moving to " <<selected << std::endl;
        if (selected > 4) {
        selected = 3;
        }
        switch (selected) {
            case 3:
                guis[3]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/backH.jpg");
                break;
            case 4:
                guis[4]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/volumeH.jpg");
                break;
        }
        break;
    }


}
void handle_up() {
    std::cout << selected<<std::endl;
    switch (depth) {
    case 0:
        switch (selected) {
        case 0:
            guis[0]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/continue.jpg");
            break;
        case 1:
            guis[1]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/options.jpg");
            break;
        case 2:
            guis[2]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/quit.jpg");
            break;
        }
        selected--;
        if (selected < 0) {
            selected = 2;
        }
        switch (selected) {
        case 0:
            guis[0]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/continueH.jpg");
            break;
        case 1:
            guis[1]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/optionsH.jpg");
            break;
        case 2:
            guis[2]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/quitH.jpg");
            break;
        }
        break;
    case 1:
        switch (selected) {
            case 3:
                guis[3]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/back.jpg");
                break;
            case 4:
                guis[4]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/options.jpg");
                break;
        }
        selected--;
        if (selected < 3) {
            selected = 4;
        }
        switch (selected) {
            case 3:
                guis[3]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/backH.jpg");
                break;
            case 4:
                guis[4]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/volumeH.jpg");
                break;
            }
        break;
    }
   


}

void show_options() {
    selected = 4;
    depth = 1;
    for (int i = 0; i < 3; i++) {
        guis[i]->SetHidden(true);
    }
    for (int i = 3; i < 7; i++) {
        guis[i]->SetHidden(false);
    }
    guis[10]->SetHidden(false);
    guis[10]->SetPosition(glm::vec2(0.5f, 0.0f));
    guis[3]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/back.jpg");
    guis[4]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/volumeH.jpg");


}

void hide_options() {
    selected = 0;
    depth = 0;
    for (int i = 0; i < 3; i++) {
        guis[i]->SetHidden(false);
    }
    for (int i = 3; i < 7; i++) {
        guis[i]->SetHidden(true);
    }
    guis[10]->SetHidden(true);
    guis[0]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/continueH.jpg");
    guis[1]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/options.jpg");
    guis[2]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/quit.jpg");
    guis[3]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/back.jpg");
    guis[4]->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/volumeH.jpg");
    

}
void handle_escp(GLFWwindow* window) {
    if (menuOn) {
        switch (depth) {
        case 0:
            menuOn = false;
            for (int i = 0; i < 3; i++) {
                guis[i]->SetHidden(true);
            }
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        case 1:
            hide_options();
            break;
        }
    }
    else {
        menuOn = true;
        for (int i = 0; i < 3; i++) {
            guis[i]->SetHidden(false);
        }
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
void handle_enter(GLFWwindow* window) {
    switch(depth) {
        case 0:
            switch (selected) {
            case 0:
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                selected = 0;
                depth = 0;
                menuOn = false;
                for (int i = 0; i < 3; i++) {
                    guis[i]->SetHidden(true);
                }
                break;
            case 1:
                show_options();
                break;
            case 2:
                //disconnect();
                break;
            }
        break;
        case 1:
            switch (selected) {
            case 4:
                break;
            case 3:
                hide_options();
                break;
            
            }
            break;
    }

}
void ClientGame::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_W:
            if (!menuOn) {
                moveForward = true;
            }
            else {
                handle_up();
            }
            break;
        case GLFW_KEY_A:
            if (!menuOn) {
                moveLeft = true;
            }
            else {
                //handleLeft()
            }
            break;
        case GLFW_KEY_S:
            if (!menuOn) {
                moveBack = true;
            }
            else {
                handle_down();
            }
            break;
        case GLFW_KEY_D:
            if (!menuOn) {
                moveRight = true;
            }
            else {
                //handle_right();
            }
            break;
        case GLFW_KEY_ESCAPE:
            handle_escp(window);
            break;
        case GLFW_KEY_ENTER:
            handle_enter(window);
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
