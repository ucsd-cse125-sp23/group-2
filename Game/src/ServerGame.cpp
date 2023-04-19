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
    //Initialize Players
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        GameData::activity[i] = true;
        GameData::positions[i] = glm::vec3(i,0,i);
        GameData::velocities[i] = glm::vec3(0,0,0);
        GameData::models[i].modelID = MODEL_ID_ROVER;
        GameData::tags[i] =
        ComponentTags::Active   +
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model;
        //TODO: Other Model Data
    }

    //Initialize Enemies
    for (int i = ENEMY_START; i < ENEMY_END; i++)
    {
        GameData::activity[i] = false;
        //TODO
    }

    //Intialize Towers
    for (int i = TOWER_START; i < TOWER_END; i++)
    {
        GameData::activity[i] = false;
        //TODO
    }

    //Intialize Resources

    //Initialize Projectiles
    for (int i = PROJECTILE_START; i < PROJECTILE_END; i++)
    {
        GameData::activity[i] = false;
        //TODO
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
const float MOVE_SPEED = 16;
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
            if (in.moveRight)
                in.print(msg);
        }
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
