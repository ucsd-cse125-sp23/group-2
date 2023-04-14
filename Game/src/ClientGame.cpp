#include "ClientGame.h"


ClientGame::ClientGame(void)
{

    //Network Initializatio
    network = new ClientNetwork();
    network->initConnection();

    //TODO Game Initialization
    gameWindow = new GameWindow(800, 600);
    gameWindow->setup();
}


//Converts from network's data to gamedata
int ClientGame::recieveData()
{
    network->recieveDeserialize(incomingData);
    return 0;
}

void ClientGame::update()
{
    //TODO Render

    // Draw nothing, see you in tutorial 2 !

    //Recieve Data
    //Recieve incoming server data into gamestate
    recieveData();

    //TODO Get and send Inputs


    //Send Data to Server
    switch (incomingData.data) {

    case ACTION_EVENT:

        printf("client received action event packet from server\n");

        network->sendActionPackets();

        break;

    default:

        //printf("error in packet types at client, incorrect type: %u\n", incomingData.data);

        break;
    }

    gameWindow->update();
    
}

void ClientGame::setup_callbacks() {

}
