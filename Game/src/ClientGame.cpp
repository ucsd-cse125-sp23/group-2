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
extern int spawntimer;
extern int moneyRes;
extern int woodRes;
extern int stoneRes;
extern int points;
extern int enemiesKilled;
extern int towersBuilt;
extern int wavetimer; //Do not render as a bar (render as a number)
extern int maxwavetimer;
extern float basehealth;
extern float maxbasehealth;
extern int curWave;
extern int numWaves;

//Nice to render but not required (only render when non-zero) should be a hidden element otherwise
extern int poweruptimer;


//Only render when one of these three is non-zero should be a hidden element otherwise
extern int moneyCost;  
extern int woodCost;
extern int stoneCost;

//not necessary to render
extern int actionCooldown;

//Change crosshair based on state values in playerstate struct
//If state == PlayerState::Upgrading vs if state == PlayerState::Build/PlayerState::Default/PlayerState::Attack
extern State state;



float health = 0.8;
float master_Volume = 1.0;
float sfx_Volume=1.0;
float music_Volume=1.0;
bool ClientGame::jumping = 0;
int ClientGame::build = 0;
int ClientGame::selected = 0;
bool ClientGame::upgrade = 0;
bool ClientGame::renderColliders = 0;
bool death_hiden = true;

void handle_death(boolean alive) {

    guis[23]->SetHidden(alive);
}

ClientGame::ClientGame(void)
{
    connectionAttempted = false;
    //TODO Game Initialization
    gameWindow = new GameWindow(RES_WIDTH, RES_HEIGHT);
    setup_callbacks();
    audioManager = new AudioManager();

    initData.id = INVALID_CLIENT_ID;
    incomingData.serverStatus = UNKNOWN_SERVER_STATUS;

    
}


//Converts from network's data to gamedata
int ClientGame::recieveData()
{
    return network->recieveDeserialize(incomingData, initData);
}

void ClientGame::update()
{
    guis[6]->SetSize(glm::vec2(0.6f * master_Volume, 0.1f));
    if (!connectionAttempted) {
        glfwPollEvents();       
        if (jumping) {
            network = new ClientNetwork();
            ClienttoServerData newPackage;
            packageData(newPackage);
            network->sendActionPackets(newPackage);
            connectionAttempted = true;
            gameWindow->wait();
        }
        else {
            return;
        }
    }
    else {
        //Recieve Data
        //Recieve incoming server data into gamestate
        recieveData();

        //Send Data to Server
        ClienttoServerData newPackage;
        packageData(newPackage);
        network->sendActionPackets(newPackage);

        //Render
        if (initData.id != INVALID_CLIENT_ID && incomingData.serverStatus != UNKNOWN_SERVER_STATUS) {
            switch (incomingData.serverStatus) {
            case ServerStatus::init:
                //waiting for other player
                break;
            case ServerStatus::game:
                for (int i = 0; i < MAX_ENTITIES; ++i) {
                    incomingData.models[i].renderCollider = renderColliders;
                }
                gameWindow->update(incomingData, initData.id);
                audioManager->update(gameWindow->getCamPosition(), glm::normalize(gameWindow->getCamDirectionVector()), glm::normalize(gameWindow->getCamUpVector()), incomingData);
                break;
            case ServerStatus::loss:
                printf("You lose\n");
                gameWindow->loss();
                break;
            case ServerStatus::win:
                printf("You won\n");
                gameWindow->win();
                break;
            default:
                printf("WTF");
                break;
            }


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
            guis[9]->SetSize(glm::vec2(0.38f * health / incomingData.healths[initData.id].maxHealth, 0.05f));

            wavetimer = incomingData.waveTimer;
            maxwavetimer = incomingData.maxWaveTimer;
            basehealth = incomingData.healths[MAX_ENTITIES_NOBASE].curHealth;
            maxbasehealth = incomingData.healths[MAX_ENTITIES_NOBASE].maxHealth;
            poweruptimer = incomingData.playerData.powerupTimers[initData.id];
            state = incomingData.playerData.playerStates[initData.id];
            actionCooldown = incomingData.playerData.actioncooldown[initData.id];
            curWave = incomingData.currentWave;
            numWaves = incomingData.numWaves;

            if (incomingData.playerData.playerStates[initData.id] == PlayerState::Build) {
                if (selected < NUM_TOWER_PREFAB && selected >= 0) {
                    moneyCost = incomingData.buildcosts[selected][ResourceType::Money];
                    stoneCost = incomingData.buildcosts[selected][ResourceType::Stone];
                    woodCost = incomingData.buildcosts[selected][ResourceType::Wood];
                }
            }
            else if (incomingData.playerData.playerStates[initData.id] == PlayerState::Upgrading && (incomingData.playerData.selectedTowerUpgrade[initData.id] != INVALID_ENTITY)) {
                    moneyCost = incomingData.playerData.selectedTowerUpgradeCost[initData.id][ResourceType::Money];
                    stoneCost = incomingData.playerData.selectedTowerUpgradeCost[initData.id][ResourceType::Stone];
                    woodCost = incomingData.playerData.selectedTowerUpgradeCost[initData.id][ResourceType::Wood];
            }
            else {
                moneyCost = 0;
                stoneCost = 0;
                woodCost = 0;
            }
            
            if (spawntimer > 0) {
                handle_death(false);
            }
            else {
                handle_death(true);
            }

            if (moneyCost != 0 || stoneCost != 0 || woodCost != 0) {
                //printf("This costs (%d, %d,%d)\n", moneyCost, stoneCost, woodCost);
            }
        }
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
    data.selected = selected;
    data.camAngleAroundPlayer = gameWindow->getCamAngle();
    data.camDirectionVector = gameWindow->getCamDirectionVector();
    data.camPosition = gameWindow->getCamPosition();
    data.upgrade = upgrade;
}

void handle_win(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    menuOn = 1;
    //guis[12]->SetHidden(false);
    guis[13]->SetHidden(false);
    guis[15]->SetHidden(false);
    guis[16]->SetHidden(false);
    depth = 2;
    selected = 5;
}
void handle_lose(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    menuOn = 1;
    //guis[12]->SetHidden(false);
    guis[14]->SetHidden(false);
    guis[15]->SetHidden(false);
    guis[16]->SetHidden(false);
    depth = 2;
    selected = 5;
}




void handle_quit() {
    exit(0);
}



void handle_down() {
    std::cout << "depth"<< depth << std::endl;
    switch (depth) {
    case 0:
        switch (selected) {
        case 0:
            guis[0]->SetTexture("../assets/gui/Buttons/continue.png");
            break;
        case 1:
            guis[1]->SetTexture("../assets/gui/Buttons/options.png");
            break;
        case 2:
            guis[2]->SetTexture("../assets/gui/Buttons/quit.png");
            break;
        }
        selected++;
        if (selected > 2) {
            selected = 0;
        }
        switch (selected) {
        case 0:
            guis[0]->SetTexture("../assets/gui/Buttons/continueH.png");
            break;
        case 1:
            guis[1]->SetTexture("../assets/gui/Buttons/optionsH.png");
            break;
        case 2:
            guis[2]->SetTexture("../assets/gui/Buttons/quitH.png");
            break;
        }
    break;
    case 1:
        switch (selected) {
            case 3:
                guis[3]->SetTexture("../assets/gui/Buttons/back.png");
                break;
            case 4:
                guis[4]->SetTexture("../assets/gui/Buttons/volume.png");
                break;
        }
        selected++;
        std::cout << "moving to " <<selected << std::endl;
        if (selected > 4) {
        selected = 3;
        }
        switch (selected) {
            case 3:
                guis[3]->SetTexture("../assets/gui/Buttons/backH.png");
                break;
            case 4:
                guis[4]->SetTexture("../assets/gui/Buttons/volumeH.png");
                break;
        }
        break;
    case 2:
        switch (selected) {
        case 5:
            guis[15]->SetTexture("../assets/gui/Buttons/new_game.png");
            break;
        case 6:
            guis[16]->SetTexture("../assets/gui/Buttons/quit.png");
            break;
        }
        selected++;
        std::cout << "moving to " << selected << std::endl;
        if (selected > 6) {
            selected = 5;
        }
        switch (selected) {
        case 5:
            guis[15]->SetTexture("../assets/gui/Buttons/new_gameH.png");
            break;
        case 6:
            guis[16]->SetTexture("../assets/gui/Buttons/quitH.png");
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
            guis[0]->SetTexture("../assets/gui/Buttons/continue.png");
            break;
        case 1:
            guis[1]->SetTexture("../assets/gui/Buttons/options.png");
            break;
        case 2:
            guis[2]->SetTexture("../assets/gui/Buttons/quit.png");
            break;
        }
        selected--;
        if (selected < 0) {
            selected = 2;
        }
        switch (selected) {
        case 0:
            guis[0]->SetTexture("../assets/gui/Buttons/continueH.png");
            break;
        case 1:
            guis[1]->SetTexture("../assets/gui/Buttons/optionsH.png");
            break;
        case 2:
            guis[2]->SetTexture("../assets/gui/Buttons/quitH.png");
            break;
        }
        break;
    case 1:
        switch (selected) {
            case 3:
                guis[3]->SetTexture("../assets/gui/Buttons/back.png");
                break;
            case 4:
                guis[4]->SetTexture("../assets/gui/Buttons/volume.png");
                break;
        }
        selected--;
        if (selected < 3) {
            selected = 4;
        }
        switch (selected) {
            case 3:
                guis[3]->SetTexture("../assets/gui/Buttons/backH.png");
                break;
            case 4:
                guis[4]->SetTexture("../assets/gui/Buttons/volumeH.png");
                break;
            }
        break;
    case 2:
        switch (selected) {
        case 5:
            guis[15]->SetTexture("../assets/gui/Buttons/new_game.png");
            break;
        case 6:
            guis[16]->SetTexture("../assets/gui/Buttons/quit.png");
            break;
        }
        selected--;
        std::cout << "moving to " << selected << std::endl;
        if (selected < 5) {
            selected = 6;
        }
        switch (selected) {
        case 5:
            guis[15]->SetTexture("../assets/gui/Buttons/new_gameH.png");
            break;
        case 6:
            guis[16]->SetTexture("../assets/gui/Buttons/quitH.png");
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
    guis[10]->SetPosition(glm::vec3(0.5f, 0.0f, 0.0f));
    guis[3]->SetTexture("../assets/gui/Buttons/back.png");
    guis[4]->SetTexture("../assets/gui/Buttons/volumeH.png");

  
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
    guis[0]->SetTexture("../assets/gui/Buttons/continueH.png");
    guis[1]->SetTexture("../assets/gui/Buttons/options.png");
    guis[2]->SetTexture("../assets/gui/Buttons/quit.png");
    guis[3]->SetTexture("../assets/gui/Buttons/back.png");
    guis[4]->SetTexture("../assets/gui/Buttons/volumeH.png");
    

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
        guis[0]->SetTexture("../assets/gui/Buttons/continueH.png");
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
                    upgrade = 0;
                }
                else {
                    upgrade = 0;
                    build = 1;
                }
                selected = 0;
            }
            break;
        case GLFW_KEY_C:
            if (renderColliders) {
                renderColliders = 0;
            }
            else {
                renderColliders = 1;
            } //Fancy one line flip
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
