#include "ClientGame.h"

GLFWwindow* createWindow()
{
    glewExperimental = true;

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "HelloWorld", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return window;
}

ClientGame::ClientGame(void)
{

    //Network Initializatio
    network = new ClientNetwork();
    network->initConnection();
    window = createWindow();

    //TODO Game Initialization
}




//Converts from network's data to gamedata
int ClientGame::recieveData()
{
    network->recieveDeserialize(incomingData);
    return 0;
}

void ClientGame::update()
{
    //TODO Render



    //Recieve Data
    //Recieve incoming server data into gamestate
    recieveData();

    //TODO Get and send Inputs


    //Send Data to Server
    switch (incomingData.data) {

    case ACTION_EVENT:

        printf("client received action event packet from server\n");

        network->sendActionPackets();

        break;

    default:

        //printf("error in packet types at client, incorrect type: %u\n", incomingData.data);

        break;
    }
    
}
