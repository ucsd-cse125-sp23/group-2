#include "ServerGame.h"

ServerGame::ServerGame(void)
{

    // set up the server network to listen 
    network = new ServerNetwork();

    gameState.playerPosition[0] = glm::vec3(0, 0, 0);
    gameState.playerPosition[1] = glm::vec3(0, 0, 0);

    gameState.playerPosition[2] = glm::vec3(0, 0, 0);
    gameState.playerPosition[3] = glm::vec3(0, 0, 0);


    debug[0] = '\0';

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

    //Debug
    printf(debug);
    debug[0] = '\0';
}



// Speed of movement in units per second
const float MOVE_SPEED = 16;
const float MOVE_DELTA = (MOVE_SPEED / TICK_RATE);
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
        //sprintf(msg, "Client: %d, Translation x: %f, Trans z: %f\n",i,  translation.x, translation.z);
        //strcat(debug, msg);

        gameState.playerPosition[i] = gameState.playerPosition[i] + translation;
        translation = glm::vec3(0, 0, 0);
    }

    
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



