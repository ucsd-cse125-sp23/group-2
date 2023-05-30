#pragma once
#include <string.h>
#include <array>
#include "graphics/core.h"
#include "EntityComponentSystem.h"
#include "GameConstants.h"

#define MAX_PACKET_SIZE 10000000
#define NUM_CLIENTS 4

enum PacketTypes {

    INIT_CONNECTION = 0,

    ACTION_EVENT = 1,

};

// Struct containing all data to be sent from client to server
struct ClienttoServerData {
    float camAngleAroundPlayer;
    glm::vec3 camDirectionVector, camPosition;
    bool moveForward, moveBack, moveLeft, moveRight;
    bool shoot;
    bool jump;
    int build;

    void print(char * buf) {
        sprintf(buf, "Moveforward: %d, Moveback: %d, MoveLeft: %d, MoveRight: %d\n", moveForward, moveBack, moveLeft, moveRight);
    }
};

// Struct containing all data to be sent from server to client
struct ServertoClientData {
    std::array<Active, MAX_ENTITIES> activity;
    std::array<Position, MAX_ENTITIES> positions;
    std::array<Model, MAX_ENTITIES> models;
    std::array<Health, MAX_ENTITIES> healths;
    int logsize;
    std::array<CombatLog, CLOG_MAXSIZE> combatLogs;
    int currentWave;
    int numWaves;
    std::array<Collider, MAX_ENTITIES> colliders;
    AllPlayerData playerData;
    std::array<std::array<int, NUM_RESOURCE_TYPES>, NUM_TOWER_PREFAB> buildcosts;
    int serverStatus;
    float waveTimer;
};

//Struct for intializing the client id
struct ServertoClientInit {
    unsigned int id;
};

//Packet wrapper for data sent across network, must define with
//previous structs
template <typename OutType>
struct Packet {

    //App Layer Data
    unsigned int packet_type;

    //Actual Game Data
    OutType data;

    void serialize(char* data) {
        memcpy(data, this, sizeof(Packet));
    }

    void deserialize(char* data) {
        memcpy(this, data, sizeof(Packet));
    }
};
