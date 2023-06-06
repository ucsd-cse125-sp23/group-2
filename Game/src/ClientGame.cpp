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
extern float spawntimer;
extern float moneyRes;
extern float woodRes;
extern float stoneRes;
extern float points;
extern float enemiesKilled;
extern float towersBuilt;



float health = 0.8;
float master_Volume = 1.0;
float sfx_Volume;
float music_Volume;
bool ClientGame::jumping = 0;
int ClientGame::build = 0;

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
        audioManager->update(gameWindow->getCamPosition(), glm::normalize(gameWindow->getCamDirectionVector()), glm::normalize(gameWindow->getCamUpVector()));
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
        glm::vec3 position = incomingData.positions[source];
        audioManager->playSound(incomingData.models[source].modelID, incomingData.soundLogs[i].sound, position);
    }
    incomingData.slogsize = 0;

    //Update Gui
    if (initData.id != INVALID_CLIENT_ID && incomingData.serverStatus != UNKNOWN_SERVER_STATUS) {
        health = incomingData.healths[initData.id].curHealth;
        ScoreCard myscore = incomingData.playerData.scores[initData.id];
        spawntimer = incomingData.playerData.spawntimers[initData.id];
        woodRes = incomingData.playerData.resources[ResourceType::Wood];
        stoneRes = incomingData.playerData.resources[ResourceType::Stone];
        moneyRes = incomingData.playerData.resources[ResourceType::Money];
        enemiesKilled = myscore.enemiesKilled;
        towersBuilt = myscore.towersBuilt;
        points = myscore.points;
        if (health < 0) {
            health = 0;
        }
        guis[9]->SetSize(glm::vec2(0.38f * health/ incomingData.healths[initData.id].maxHealth, 0.05f));

    }


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

void handle_win(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    menuOn = 1;
    guis[12]->SetHidden(false);
    guis[13]->SetHidden(false);
    guis[15]->SetHidden(false);
    guis[16]->SetHidden(false);
    depth = 2;
    selected = 5;
}
void handle_lose(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    menuOn = 1;
    guis[12]->SetHidden(false);
    guis[14]->SetHidden(false);
    guis[15]->SetHidden(false);
    guis[16]->SetHidden(false);
    depth = 2;
    selected = 5;
}


void handle_quit() {}


void handle_down() {
    std::cout << "depth"<< depth << std::endl;
    switch (depth) {
    case 0:
        switch (selected) {
        case 0:
            guis[0]->SetTexture("../assets/gui/Buttons/continue.jpg");
            break;
        case 1:
            guis[1]->SetTexture("../assets/gui/Buttons/options.jpg");
            break;
        case 2:
            guis[2]->SetTexture("../assets/gui/Buttons/quit.jpg");
            break;
        }
        selected++;
        if (selected > 2) {
            selected = 0;
        }
        switch (selected) {
        case 0:
            guis[0]->SetTexture("../assets/gui/Buttons/continueH.jpg");
            break;
        case 1:
            guis[1]->SetTexture("../assets/gui/Buttons/optionsH.jpg");
            break;
        case 2:
            guis[2]->SetTexture("../assets/gui/Buttons/quitH.jpg");
            break;
        }
    break;
    case 1:
        switch (selected) {
            case 3:
                guis[3]->SetTexture("../assets/gui/Buttons/back.jpg");
                break;
            case 4:
                guis[4]->SetTexture("../assets/gui/Buttons/volume.jpg");
                break;
        }
        selected++;
        std::cout << "moving to " <<selected << std::endl;
        if (selected > 4) {
        selected = 3;
        }
        switch (selected) {
            case 3:
                guis[3]->SetTexture("../assets/gui/Buttons/backH.jpg");
                break;
            case 4:
                guis[4]->SetTexture("../assets/gui/Buttons/volumeH.jpg");
                break;
        }
        break;
    case 2:
        switch (selected) {
        case 5:
            guis[15]->SetTexture("../assets/gui/Buttons/new_game.jpg");
            break;
        case 6:
            guis[16]->SetTexture("../assets/gui/Buttons/quit.jpg");
            break;
        }
        selected++;
        std::cout << "moving to " << selected << std::endl;
        if (selected > 6) {
            selected = 5;
        }
        switch (selected) {
        case 5:
            guis[15]->SetTexture("../assets/gui/Buttons/new_gameH.jpg");
            break;
        case 6:
            guis[16]->SetTexture("../assets/gui/Buttons/quitH.jpg");
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
            guis[0]->SetTexture("../assets/gui/Buttons/continue.jpg");
            break;
        case 1:
            guis[1]->SetTexture("../assets/gui/Buttons/options.jpg");
            break;
        case 2:
            guis[2]->SetTexture("../assets/gui/Buttons/quit.jpg");
            break;
        }
        selected--;
        if (selected < 0) {
            selected = 2;
        }
        switch (selected) {
        case 0:
            guis[0]->SetTexture("../assets/gui/Buttons/continueH.jpg");
            break;
        case 1:
            guis[1]->SetTexture("../assets/gui/Buttons/optionsH.jpg");
            break;
        case 2:
            guis[2]->SetTexture("../assets/gui/Buttons/quitH.jpg");
            break;
        }
        break;
    case 1:
        switch (selected) {
            case 3:
                guis[3]->SetTexture("../assets/gui/Buttons/back.jpg");
                break;
            case 4:
                guis[4]->SetTexture("../assets/gui/Buttons/volume.jpg");
                break;
        }
        selected--;
        if (selected < 3) {
            selected = 4;
        }
        switch (selected) {
            case 3:
                guis[3]->SetTexture("../assets/gui/Buttons/backH.jpg");
                break;
            case 4:
                guis[4]->SetTexture("../assets/gui/Buttons/volumeH.jpg");
                break;
            }
        break;
    case 2:
        switch (selected) {
        case 5:
            guis[15]->SetTexture("../assets/gui/Buttons/new_game.jpg");
            break;
        case 6:
            guis[16]->SetTexture("../assets/gui/Buttons/quit.jpg");
            break;
        }
        selected--;
        std::cout << "moving to " << selected << std::endl;
        if (selected < 5) {
            selected = 6;
        }
        switch (selected) {
        case 5:
            guis[15]->SetTexture("../assets/gui/Buttons/new_gameH.jpg");
            break;
        case 6:
            guis[16]->SetTexture("../assets/gui/Buttons/quitH.jpg");
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
    for (int i = 3; i < 6; i++) {
        guis[i]->SetHidden(false);
    }
    guis[10]->SetHidden(false);
    guis[10]->SetPosition(glm::vec3(0.5f, 0.0f, 0.0f));
    guis[3]->SetTexture("../assets/gui/Buttons/back.jpg");
    guis[4]->SetTexture("../assets/gui/Buttons/volumeH.jpg");

  
}

void hide_options() {
    selected = 0;
    depth = 0;
    for (int i = 0; i < 3; i++) {
        guis[i]->SetHidden(false);
    }
    for (int i = 3; i < 6; i++) {
        guis[i]->SetHidden(true);
    }
    guis[10]->SetHidden(true);
    guis[0]->SetTexture("../assets/gui/Buttons/continueH.jpg");
    guis[1]->SetTexture("../assets/gui/Buttons/options.jpg");
    guis[2]->SetTexture("../assets/gui/Buttons/quit.jpg");
    guis[3]->SetTexture("../assets/gui/Buttons/back.jpg");
    guis[4]->SetTexture("../assets/gui/Buttons/volumeH.jpg");
    

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
        guis[0]->SetTexture("../assets/gui/Buttons/continueH.jpg");
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

void handle_left() {

    switch (depth) {
    case 0:
        break;
    case 1:
        switch (selected) {
        case 4:
            std::cout << "master volume: " << master_Volume << std::endl;
            if (master_Volume > 0) {
                master_Volume = master_Volume - 0.05;
            }
            else {
                master_Volume = 0;
            }
          //guis[6]->SetPosition(glm::vec3(0.5f, 0.0f, -0.2f));
            guis[6]->SetSize(glm::vec2(0.6f * master_Volume, 0.1f));
            break;
        case 3:
           
            break;

        }
        break;
    }

}

void handle_right() {
    switch (depth) {
    case 0:
        break;
    case 1:
        switch (selected) {
        case 4:
            std::cout << "master volume: " << master_Volume << std::endl;
            if (master_Volume < 1) {
                master_Volume = master_Volume + 0.05;
            }
            else {
                master_Volume = 1;
            }
            //guis[6]->SetPosition(glm::vec3(0.5f, 0.0f, -0.2f));
            guis[6]->SetSize(glm::vec2(0.6f * master_Volume, 0.1f));
            break;
        case 3:

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
                moveForward = false;
                handle_up();
            }
            break;
        case GLFW_KEY_A:
            if (!menuOn) {
                moveLeft = true;
            }
            else {
                moveLeft = false;
                handle_left();
            }
            break;
        case GLFW_KEY_S:
            if (!menuOn) {
                moveBack = true;
            }
            else {
                moveBack = false;
                handle_down();
               
            }
            break;
        case GLFW_KEY_D:
            if (!menuOn) {
                moveRight = true;
            }
            else {
                moveRight = false;
                handle_right();
               
            }
            break;
        case GLFW_KEY_ESCAPE:
            moveRight = false;
            moveLeft = false;
            moveBack = false;
            moveForward = false;
            build = false; 
            jumping = false;
            handle_escp(window);
            break;
        case GLFW_KEY_SPACE:
            if (!menuOn) {
                jumping = true;
            }
            else {
                jumping = false;
            }
            break;
        case GLFW_KEY_E:
            if (!menuOn) {
                if (build != 0) {
                    build = 0;
                }
                else {
                    build = 1;
                }
            }
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
