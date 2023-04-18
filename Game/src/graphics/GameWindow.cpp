#include "GameWindow.h"

void setup_opengl_settings() {
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

GameWindow::GameWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        //return NULL;
    }

    windowTitle = "Mars-Rover Client";

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window.
    window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        //return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewInit();

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // set up the camera
    cam = new Camera();
    cam->SetAspect(float(width) / float(height));

    // Call the resize callback to make sure things get drawn immediately.
    resizeCallback(window, width, height);

    setup();
}

GameWindow::~GameWindow() {
    cleanUp();
    // Destroy the window.
    glfwDestroyWindow(window);
    // Terminate GLFW.
    glfwTerminate();

}

void GameWindow::cleanUp() {
    delete player;

    glDeleteProgram(shaderProgram);
}
void GameWindow::setup() {
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Initialize the shader program; exit if initialization fails.
    if (!initializeProgram()) exit(EXIT_FAILURE);
    // Initialize objects/pointers for rendering; exit if initialization fails.
    if (!initializeObjects()) exit(EXIT_FAILURE);
}
bool GameWindow::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    //cubeShader = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}


void GameWindow::resizeCallback(GLFWwindow* window, int w, int h) {
    width = w;
    height = h;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    cam->SetAspect(float(width) / float(height));
}
bool GameWindow::initializeObjects() {
    player = new Player();
    return true;
}

void GameWindow::idleCallback(ServertoClientData& incomingData) {
    cam->Update();
    player->update(incomingData.playerTranslation);
}

void GameWindow::displayCallback() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    player->draw(cam->GetViewProjectMtx(), shaderProgram);
    glfwPollEvents();
    glfwSwapBuffers(window);
}
void GameWindow::update(ServertoClientData & incomingData) {
    displayCallback();
    idleCallback(incomingData);
}

// helper to reset the camera
void GameWindow::resetCamera() {
    cam->Reset();
    cam->SetAspect(float(width) / float(height));
}