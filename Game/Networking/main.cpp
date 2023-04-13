#include "ServerGame.h"
#include "ClientGame.h"
// used for multi-threading
#include <process.h>

void serverLoop(void*);
void clientLoop(void);

ServerGame* server;
ClientGame* client1;
ClientGame* client2;
ClientGame* client3;
ClientGame* client4;
int main()
{

    // initialize the server
    server = new ServerGame();
    // create thread with arbitrary argument for the run function
    _beginthread(serverLoop, 0, (void*)12);

    // initialize the clients
    client1 = new ClientGame();
    client2 = new ClientGame();
    client3 = new ClientGame();
    client4 = new ClientGame();
    clientLoop();
}

void serverLoop(void* arg)
{
    while (true)
    {
        server->update();
    }
}

void clientLoop()
{
    while (true)
    {
        
        //do game stuff
       client1->update();
       client2->update();
       client3->update();
       client4->update();
       
    }
}