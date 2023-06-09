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
    window = glfwCreateWindow(width, height, windowTitle, glfwGetPrimaryMonitor(), NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        //return NULL;
    }

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewInit();

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // set up the camera
    //cam = new Camera();
    //cam->SetAspect(float(width) / float(height));

    // Call the resize callback to make sure things get drawn immediately.
    resizeCallback(window, width, height);

    setup(window);
}

GameWindow::~GameWindow() {
    cleanUp();
    // Destroy the window.
    glfwDestroyWindow(window);
    // Terminate GLFW.
    glfwTerminate();

}

void GameWindow::cleanUp() {
    shaderProgram->cleanup();
}

void GameWindow::setup(GLFWwindow* window) {

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
    if (!initializeObjects(window)) exit(EXIT_FAILURE);
}
bool GameWindow::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    //cubeShader = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    guiProgram = new Shader("shaders/gui.vert", "shaders/gui.frag");

    return true;
}


void GameWindow::resizeCallback(GLFWwindow* window, int w, int h) {
    // Set the viewport size.
    glViewport(0, 0, w, h);

    //cam->SetAspect(float(width) / float(height));
}
bool GameWindow::initializeObjects(GLFWwindow* window) {
    gameWorld = new GameWorld();
    gameWorld->init(window);
    return true;
}

void GameWindow::idleCallback(ServertoClientData& incomingData, int id) {
    gameWorld->update(incomingData, id);
}

void GameWindow::displayCallback() {
    glfwMakeContextCurrent(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //player->draw(cam->GetViewProjectMtx(), shaderProgram);
    gameWorld->draw(guiProgram, width, height);
    glfwPollEvents();
    glfwSwapBuffers(window);
}
void GameWindow::update(ServertoClientData& incomingData, int id) {
    displayCallback();
    idleCallback(incomingData, id);
}

void GameWindow::win() {
    glfwMakeContextCurrent(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    stbi_set_flip_vertically_on_load(true);
    GUIElement* loadingScreen = new GUIElement();
    loadingScreen->SetHidden(false);
    loadingScreen->SetName("loading screen");
    loadingScreen->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    loadingScreen->SetSize(glm::vec2(2.0f, 1.6f));
    loadingScreen->SetTexture("../assets/screens/Victory.png");
    loadingScreen->SetTransparency(1.0);
    loadingScreen->draw(glm::mat4(1.0), guiProgram);

    glfwPollEvents();
    glfwSwapBuffers(window);
}

void GameWindow::loss() {
    glfwMakeContextCurrent(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    stbi_set_flip_vertically_on_load(true);
    GUIElement* loadingScreen = new GUIElement();
    loadingScreen->SetHidden(false);
    loadingScreen->SetName("loading screen");
    loadingScreen->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    loadingScreen->SetSize(glm::vec2(2.0f, 1.6f));
    loadingScreen->SetTexture("../assets/screens/Game_Over.png");
    loadingScreen->SetTransparency(1.0);
    loadingScreen->draw(glm::mat4(1.0), guiProgram);

    glfwPollEvents();
    glfwSwapBuffers(window);
}


