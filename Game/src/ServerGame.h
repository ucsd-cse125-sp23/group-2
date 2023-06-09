#pragma once
#include "ServerNetwork.h"
#include <vector>
#include <queue>
#include <array>
#include "EntityComponentSystem.h"
#include "GameConstants.h"
#include "Prefabs.h"
#include <ctime>
#include "PoissonDisk.h"

#define DEBUG_BUFFER 100000
namespace ECS = EntityComponentSystem;
class ServerGame
{

public:

    queue<ClienttoServerData> incomingDataLists[NUM_CLIENTS];

    ServertoClientData gameState;

    ServerGame(void);
    ~ServerGame(void);

    void receiveFromClients();

    void packageData(ServertoClientData& data);

    void initializeGame();

    void handleInputs();

    void update();

    void waveSpawner();

    void sendPackets();

    void initPrefabs();

private:


    // The ServerNetwork object 
    ServerNetwork* network;

    //Debug Buffer
    char debug[DEBUG_BUFFER];

    void asciiView();

    unsigned int curTick;

    void changeState(Entity e, State post);

    void playerAttack(Entity i, glm::vec3& camdir, glm::vec3& campos);

    void playerBuild(Entity i, glm::vec3& camdir, glm::vec3& campos);

    void initPlayers();

    void initWaves();

    void initResources();

    void initBase();

    void checkStatus();

    Entity playerUpgrade(Entity i, glm::vec3& camdir, glm::vec3& campos, float range);

    Entity home;

    uint16_t clientsConnected;

    enum ServerStatus {
        init,
        game,
        win,
        loss
    };

    ServerStatus currentStatus;
};