#include "src/ServerGame.h"
#include "src/ClientGame.h"
#include <chrono>
#include <ctime>   
#include <iostream>
// used for multi-threading
#include <process.h>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
  
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
    //client2 = new ClientGame();
    //client3 = new ClientGame();
    //client4 = new ClientGame();
    clientLoop();
}

void serverLoop(void* arg)
{
    double spertick = 1.0 / TICK_RATE;
    std::chrono::duration<double> ticklength = std::chrono::duration<double>(spertick);
    auto start = std::chrono::system_clock::now();
    while (true)
    {
        // Some computation here
        server->update();
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::chrono::duration<double> sleeptime = ticklength - elapsed;
        if (sleeptime > 0s) {
            std::this_thread::sleep_for(sleeptime);
        }
        else {
            printf("server update took too long!\n");
        }
        start = std::chrono::system_clock::now();
        server->sendPackets();
    }
}

void clientLoop()
{
    
    while (true)
    {
        //do game stuff
       client1->update();
       //client2->update();
       //client3->update();
       //client4->update();
       
    }
    
}