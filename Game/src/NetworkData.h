#pragma once
#include <string.h>
#include "graphics/core.h"

#define MAX_PACKET_SIZE 1000000

struct GameData;

enum PacketTypes {

    INIT_CONNECTION = 0,

    ACTION_EVENT = 1,

};

// Struct containing all data to be sent from client to server
struct ClienttoServerData {
    bool moveForward, moveBack, moveLeft, moveRight;

    void print() {
        printf("Moveforward: %d, Moveback: %d, MoveLeft: %d, MoveRIght: %d\n", moveForward, moveBack, moveLeft, moveRight);
    }
};

// Struct containing all data to be sent from server to client
struct ServertoClientData {
    glm::vec3 playerPosition;
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
