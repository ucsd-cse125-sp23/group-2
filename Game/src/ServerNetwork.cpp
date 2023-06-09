#include "ServerNetwork.h"

ServerNetwork::ServerNetwork(void)
{

    // create WSADATA object
    WSADATA wsaData;

    // our sockets for the server
    ListenSocket = INVALID_SOCKET;

    for (int i = 0; i < NUM_CLIENTS; ++i) {
        sessions[i] = INVALID_SOCKET;
    }

    // address info for the server to listen to
    struct addrinfo* result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }

    // set address information
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;    // TCP connection!!!
    hints.ai_flags = AI_PASSIVE;
    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Set the mode of the socket to be nonblocking
    u_long iMode = 1;
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

    if (iResult == SOCKET_ERROR) {
        printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // no longer need address information
    freeaddrinfo(result);

    // start listening for new clients attempting to connect
    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }
}

// accept new connections
bool ServerNetwork::acceptNewClient()
{
    // if client waiting, accept the connection and save the socket
    SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);

    if (ClientSocket != INVALID_SOCKET)
    {
        //disable nagle on the client's socket
        char value = 1;
        setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

        // send action packet
        const unsigned int packet_size = sizeof(Packet<ServertoClientInit>);
        char packet_data[packet_size];

        Packet<ServertoClientInit> packet;
        packet.packet_type = INIT_CONNECTION;
        
        unsigned int id = insertClient(ClientSocket);
        packet.data = ServertoClientInit{ id };

        packet.serialize(packet_data);


        if (id == NUM_CLIENTS) {
            return false;
        }
        int iSendResult = NetworkServices::sendMessage(ClientSocket, packet_data, packet_size);

        if (iSendResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            sessions[id] = INVALID_SOCKET;
        }
        

        // insert new client into session id table (return false if full)
        return true;
    }

    return false;
}

// recieve incoming data
int ServerNetwork::receiveDeserialize(queue<ClienttoServerData>(& incomingDataLists)[NUM_CLIENTS])
{
    Packet<ClienttoServerData> packet;

    for (unsigned int i = 0; i < NUM_CLIENTS; ++i)
    {
        if (sessions[i] != INVALID_SOCKET) {
            int data_length = receivePackets(i, network_data);

            if (data_length <= 0)
            {
                //no data recieved
                continue;
            }

            int data_red = 0;
            while (data_red < (unsigned int)data_length)
            {
                packet.deserialize(&(network_data[data_red]));
                data_red += sizeof(Packet<ClienttoServerData>);

                incomingDataLists[i].push(ClienttoServerData{ packet.data });

                //printf("error in packet types at server, incorrect type: %u\n", packet.data.data);
            }
        }
    }
    return 0;
}

// receive incoming packets from certain client
int ServerNetwork::receivePackets(unsigned int client_id, char* recvbuf)
{
    if (client_id < NUM_CLIENTS)
    {
        SOCKET currentSocket = sessions[client_id];
        iResult = NetworkServices::receiveMessage(currentSocket, recvbuf, MAX_PACKET_SIZE);
        if (iResult == 0)
        {
            printf("Connection closed\n");
            closesocket(currentSocket);
            sessions[client_id] = INVALID_SOCKET;
        }


        //Change later if things break
        else if (iResult == WSAECONNRESET) {
            printf("Connection reset\n");
            closesocket(currentSocket);
            sessions[client_id] = INVALID_SOCKET;
        }

        //Change later if things break
        else if (iResult == WSAECONNABORTED) {
            printf("Connection aborted\n");
            closesocket(currentSocket);
            sessions[client_id] = INVALID_SOCKET;
        }
        return iResult;
    }
    return 0;
}

// sends data to all clients
void ServerNetwork::sendActionPackets(ServertoClientData & outgoingData)
{

    gameStatePacket.packet_type = ACTION_EVENT;

    gameStatePacket.data = outgoingData;

    gameStatePacket.serialize(packet_data);

    sendToAll(packet_data, sizeof(Packet<ServertoClientData>));
}

// send packets to all clients
void ServerNetwork::sendToAll(char* packets, int totalSize)
{
    SOCKET currentSocket;
    int iSendResult;

    for (unsigned int i = 0; i < NUM_CLIENTS; ++i)
    {
        currentSocket = sessions[i];

        if (currentSocket != INVALID_SOCKET) {
            iSendResult = NetworkServices::sendMessage(currentSocket, packets, totalSize);

            if (iSendResult == SOCKET_ERROR)
            {
                printf("send failed with error: %d\n", WSAGetLastError());
                if (WSAGetLastError() != WSAEWOULDBLOCK) {
                    closesocket(currentSocket);
                    sessions[i] = INVALID_SOCKET;
                }
            }
        }
    }
}

int ServerNetwork::insertClient(SOCKET & ClientSocket)
{
    for (int i = 0; i < NUM_CLIENTS; ++i) {
        if (sessions[i] == INVALID_SOCKET) {
            printf("Inserting new client with id %d\n", i);
            sessions[i] = ClientSocket;
            return i;
        }
    }
    return NUM_CLIENTS;
}
