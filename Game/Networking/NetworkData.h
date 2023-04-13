#pragma once
#include <string.h>

#define MAX_PACKET_SIZE 1000000

struct GameData;

enum PacketTypes {

    INIT_CONNECTION = 0,

    ACTION_EVENT = 1,

};

struct ClienttoServerData {
    unsigned int data;
};

struct ServertoClientData {
    unsigned int data;
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
