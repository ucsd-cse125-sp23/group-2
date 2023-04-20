#include "ServerGame.h"

ServerGame::ServerGame(void)
{

    // set up the server network to listen
    network = new ServerNetwork();

    initializeGame();

    debug[0] = '\0';

}

//Populate Component Arrays
void ServerGame::initializeGame()
{
    initPlayers();
    //initEnemies();
    //initTowers();
    //initResources();
    //initProjectiles();
}

void ServerGame::initPlayers() 
{
    //Initialize Players
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        GameData::activity[i] = true;
        GameData::positions[i] = glm::vec3(i, 0, i);
        GameData::velocities[i] = glm::vec3(0, 0, 0);
        GameData::models[i].modelID = MODEL_ID_ROVER;
        GameData::tags[i] =
            ComponentTags::Active +
            ComponentTags::Position +
            ComponentTags::Velocity +
            ComponentTags::Model;
        //TODO: Other Model Data
    }
}

void ServerGame::initEnemies() 
{

    //Create Path (TEMP FOR TESTING) TODO: REMOVE FOR FINAL VERSION
    glm::vec3 testPath[PATH_LENGTH] = { glm::vec3(0,0,0), glm::vec3(0,0,-10), glm::vec3(0,0,0), glm::vec3(0,0,-10), glm::vec3(0,0,0), glm::vec3(0,0,-10) };

    //TEMP INIT FOR TESTING (1 enemy) TODO: REMOVE FOR FINAL VERSION
    GameData::activity[ENEMY_START] = true;
    GameData::positions[ENEMY_START] = glm::vec3(0, 0, 0);
    memcpy(GameData::pathStructs[ENEMY_START].pathNodes, testPath, sizeof(GameData::pathStructs[ENEMY_START].pathNodes));
    GameData::pathStructs[ENEMY_START].currentNode = 0;
    GameData::pathStructs[ENEMY_START].moveSpeed = 0.1;
    GameData::tags[ENEMY_START] =
        ComponentTags::Active   +
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::PathData +
        ComponentTags::Model;

    //Initialize Enemies (TODO: REMOVE +1 ON i INIT FOR FINAL VERSION)
    for (int i = ENEMY_START+1; i < ENEMY_END; i++)
    {
        GameData::activity[i] = false;
        GameData::positions[i] = glm::vec3(0, 0, 0);
        memcpy(GameData::pathStructs[i].pathNodes, testPath, sizeof(GameData::pathStructs[ENEMY_START].pathNodes));
        GameData::pathStructs[i].currentNode = 0;
        GameData::pathStructs[i].moveSpeed = 0.1;
        GameData::tags[i] =
            ComponentTags::Active +
            ComponentTags::Position +
            ComponentTags::Velocity +
            ComponentTags::PathData +
            ComponentTags::Model;
    }


    /*
    for (int i = ENEMY_START; i < ENEMY_END; i++)
    {
        GameData::activity[i] = false;
    }
    */
}

//TODO
void ServerGame::initTowers() 
{
    for (int i = TOWER_START; i < TOWER_END; i++)
    {
        GameData::activity[i] = false;
    }
}

//TODO
void ServerGame::initResources() 
{

}

//TODO
void ServerGame::initProjectiles() 
{
    for (int i = PROJECTILE_START; i < PROJECTILE_END; i++)
    {
        GameData::activity[i] = false;
    }
}

void ServerGame::update()
{
    // get new clients
    if (network->acceptNewClient())
    {
        printf("New client has been connected to the server\n");
    }

    //Receve Input
    receiveFromClients();

    handleInputs();

    EntityComponentSystem::update();

    //sendPackets();

    //Debug
    printf(debug);
    debug[0] = '\0';
}



// Speed of movement in units per second
const float MOVE_SPEED = 0.1;
const float MOVE_DELTA = (MOVE_SPEED / TICK_RATE);
void ServerGame::handleInputs()
{

    char msg[100];
    msg[0] = '\0';

    for(int i = 0; i < NUM_CLIENTS; i++)
    {
        while (!incomingDataLists[i].empty())
        {
            ClienttoServerData in = incomingDataLists[i].front();
            GameData::velocities[i] = glm::vec3(0,GameData::velocities[i].y,0);
            if (in.moveForward)
                GameData::velocities[i].z = -1 * MOVE_SPEED;
            if (in.moveLeft)
                GameData::velocities[i].x = -1 * MOVE_SPEED;
            if (in.moveBack)
                GameData::velocities[i].z = MOVE_SPEED;
            if (in.moveRight)
                GameData::velocities[i].x = MOVE_SPEED;

            incomingDataLists[i].pop();
        }
        //in.print(msg);
    }

}


/*
void ServerGame::step()
{
    char msg[100];
    msg[0] = '\0';
    glm::vec3 translation(0, 0, 0);
    for (int i = 0; i < NUM_CLIENTS; ++i) {
        while (!incomingDataLists[i].empty()) {
            ClienttoServerData in = incomingDataLists[i].front();
            if (in.moveForward)
                translation.z = -1 * MOVE_DELTA;
            if (in.moveLeft)
                translation.x = -1 * MOVE_DELTA;
            if (in.moveBack)
                translation.z = MOVE_DELTA;
            if (in.moveRight)
                translation.x = MOVE_DELTA;
            incomingDataLists[i].pop();
        }
        //in.print(msg);
        sprintf(msg, "Client: %d, Translation x: %f, Trans z: %f\n",i,  translation.x, translation.z);
        strcat(debug, msg);

        gameState.playerPosition[i] = gameState.playerPosition[i] + translation;
        translation = glm::vec3(0, 0, 0);
    }


}
*/

void ServerGame::sendPackets()
{

    //Send Data to Clients
    ServertoClientData newPackage;
    packageData(newPackage);
    //std::cout << newPackage.moveForward << "\n";
    network->sendActionPackets(newPackage);
}

void ServerGame::receiveFromClients()
{

    network->receiveDeserialize(incomingDataLists);

}

void ServerGame::packageData(ServertoClientData& data)
{
    data.positions = GameData::positions;
    data.models = GameData::models;
    data.activity = GameData::activity;
}
