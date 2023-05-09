#pragma once
#include "Player.h"
#include "Mob.h"
#include "Skybox.h"
#include "Shader.h"
#include "ObjectModel.h"
#include "Camera.h"
#include "../GameConstants.h"
#include "../NetworkData.h"
class GameWorld {

private:

    std::array <Player*, NUM_CLIENTS> players;
    std::array <Mob*, NUM_ENEMIES> mobs;
    unsigned int playerIndex, mobIndex;
    Skybox* env;

    Camera* cam;
    static float prevX, prevY, currX, currY, scrollY;
    //static int mouseDX, mouseDY;

public:
    void init();

    //
    void update(ServertoClientData& incomingData, int id);

    //render all active entities
    void draw(Shader* shader, Shader* skyboxShader);
    
    float getCamAngle() { return cam->getAngleAroundPlayer(); };
    glm::vec3 getCamDirectionVector() { return cam->getDirectionVector(); };
    glm::vec3 getCamPosition() { return cam->getCameraPosition(); };
    //callbacks - for interaction
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow* window, double currX, double currY);
};