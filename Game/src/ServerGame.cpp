#include "ServerGame.h"

ServerGame::ServerGame(void)
{

    // set up the server network to listen 
    network = new ServerNetwork();

    gameState.playerPosition = glm::vec3(0, 0, 0);

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

    //Step Game
    step();
}



// Speed of movement in units per second
const float MOVE_SPEED = 16;
const float MOVE_DELTA = (MOVE_SPEED / TICK_RATE);
void ServerGame::step()
{
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
    }

    gameState.playerPosition = gameState.playerPosition + translation;
}

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
    data = gameState;
}



