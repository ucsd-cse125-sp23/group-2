#include "ServerGame.h"

unsigned int ServerGame::client_id;


ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork();

}

void ServerGame::update()
{

    // get new clients
    if (network->acceptNewClient(client_id))
    {
        printf("client %d has been connected to the server\n", client_id);

        client_id++;
    }

    //Receve Input
    receiveFromClients();

    //Step Game
    step();

    //Send Data to Clients
    ServertoClientData newPackage;
    packageData(newPackage);
    //std::cout << newPackage.moveForward << "\n";
    network->sendActionPackets(newPackage);
}

void ServerGame::step()
{
    glm::vec3 translation(0, 0, 0);

    while (!incomingDataList.empty()) {
        ClienttoServerData in = incomingDataList.front();
        if (in.moveForward)
            translation.z += -.02;
        if (in.moveLeft)
            translation.x += -.02;
        if (in.moveBack)
            translation.z += .02;
        if (in.moveRight)
            translation.x += .02;
        incomingDataList.pop();
    }

    gameState.playerTranslation = translation;
}

void ServerGame::receiveFromClients()
{

    network->receiveDeserialize(incomingDataList);

}

void ServerGame::packageData(ServertoClientData& data)
{
    data = gameState;
}



