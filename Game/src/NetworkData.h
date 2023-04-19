#pragma once
#include <string.h>
#include <array>
#include "graphics/core.h"
#include "EntityComponentSystem.h"
#include "GameConstants.h"

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {

    INIT_CONNECTION = 0,

    ACTION_EVENT = 1,

};

// Struct containing all data to be sent from client to server
struct ClienttoServerData {
    bool moveForward, moveBack, moveLeft, moveRight;

    void print(char * buf) {
        sprintf(buf, "Moveforward: %d, Moveback: %d, MoveLeft: %d, MoveRIght: %d\n", moveForward, moveBack, moveLeft, moveRight);
    }
};

// Struct containing all data to be sent from server to client
struct ServertoClientData {
    std::array<Active, MAX_ENTITIES> activity;
    std::array<Position, MAX_ENTITIES> positions;
    std::array<Model, MAX_ENTITIES> models;

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
