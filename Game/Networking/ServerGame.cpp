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

    receiveFromClients();
}

void ServerGame::receiveFromClients()
{

    network->receiveDeserialize(incomingDataList);

    for (ClienttoServerData in : incomingDataList) {
        switch (in.data) {

        case INIT_CONNECTION:

            printf("server received init packet from client\n");

            network->sendActionPackets();

            break;

        case ACTION_EVENT:

            printf("server received action event packet from client\n");

            network->sendActionPackets();

            break;

        default:

            printf("error in packet types at server, incorrect type: %u\n", in.data);

            break;
        }
    }

}



