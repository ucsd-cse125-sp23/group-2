#include "ClientNetwork.h"

ClientNetwork::ClientNetwork(void) {

    // create WSADATA object
    WSADATA wsaData;

    // socket
    ConnectSocket = INVALID_SOCKET;

    // holds address info for socket to connect to
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }



    // set address info
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!
    //resolve server address and port
    iResult = getaddrinfo(SERVER_IP, DEFAULT_PORT, &hints, &result);

    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            exit(1);
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        if (iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            printf("The server is down... did not connect");
        }
    }



    // no longer need address info for server
    freeaddrinfo(result);



    // if connection failed
    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
        exit(1);
    }

    // Set the mode of the socket to be nonblocking
    u_long iMode = 1;

    iResult = ioctlsocket(ConnectSocket, FIONBIO, &iMode);
    if (iResult == SOCKET_ERROR)
    {
        printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);
    }
    //disable nagle
    char value = 1;
    setsockopt(ConnectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

}

int ClientNetwork::receivePackets(char* recvbuf)
{
    iResult = NetworkServices::receiveMessage(ConnectSocket, recvbuf, MAX_PACKET_SIZE);

    if (iResult == 0)
    {
        printf("Connection closed\n");
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);
    }

    return iResult;
}

int ClientNetwork::initConnection()
{

    // send init packet
    const unsigned int packet_size = sizeof(Packet<ClienttoServerData>);
    char packet_data[packet_size];

    //Create packet and store data (Packet type for now)
    Packet<ClienttoServerData> packet;
    packet.packet_type = INIT_CONNECTION;
    packet.data = ClienttoServerData{ INIT_CONNECTION };


    //Converts into sendable bytes
    packet.serialize(packet_data);

    //Send the bytes
    return NetworkServices::sendMessage(this->ConnectSocket, packet_data, packet_size);
}

int ClientNetwork::recieveDeserialize(ServertoClientData& incomingData, ServertoClientInit & initData)
{
    Packet<ServertoClientData> datapacket;
    Packet<ServertoClientInit> initpacket;

    int data_length = receivePackets(network_data);

    if (data_length <= 0)
    {
        //no data recieved
        return 0;
    }

    int i = 0;
    while (i < (unsigned int)data_length)
    {
        datapacket.deserialize(&(network_data[i]));
        switch (datapacket.packet_type) {
        case INIT_CONNECTION:
            initpacket.deserialize(&(network_data[i]));
            initData = initpacket.data;
            i += sizeof(Packet<ServertoClientInit>);
            break;
        case ACTION_EVENT:
            incomingData = datapacket.data;
            i += sizeof(Packet<ServertoClientData>);
            break;
        }
    }
    return 0;
}

// 
void ClientNetwork::sendActionPackets(ClienttoServerData& outgoingData)
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet<ClienttoServerData>);
    char packet_data[packet_size];

    Packet<ClienttoServerData> packet;
    packet.packet_type = ACTION_EVENT;
    packet.data = ClienttoServerData{outgoingData};
    packet.serialize(packet_data);

    NetworkServices::sendMessage(ConnectSocket, packet_data, packet_size);
}