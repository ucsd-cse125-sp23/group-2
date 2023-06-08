#pragma once
#include "RenderEntity.h"
#include "Skybox.h"
#include "Shader.h"
#include "Camera.h"
#include "Effects.h"
#include "Health.h"
#include "../GameConstants.h"
#include "../NetworkData.h"
#include <stack>
class GameWorld {

private:
    std::array <Shader*, NUM_MODELS> shaders;
    std::array <ObjectModel*, NUM_MODELS> models;
    Shader* healthShader;
    int currID;
    std::array <Cube*, MAX_ENTITIES> AABBs;
    std::array <RenderEntity*, MAX_ENTITIES> entities;
    std::array <HealthBar*, MAX_ENTITIES> healths;
    std::stack <CombatLog> newCLogs;
    std::stack <SoundLog> newSLogs;

    EffectSystem* effect;
    Skybox* env;

    Camera* cam;
    float currWaveTimer;
    static float prevX, prevY, currX, currY, scrollY;
    float playerHealth;
    bool shakeScreen, screenShakeOn, playerJumped;
    float startTime;
    //static int mouseDX, mouseDY;

public:
    void init();

    //
    void update(ServertoClientData& incomingData, int id);

    //render all active entities
    void draw();
    
    float getCamAngle() { return cam->getAngleAroundPlayer(); };
    glm::vec3 getCamDirectionVector() { return cam->getDirectionVector(); };
    glm::vec3 getCamUpVector() { return cam->getUpVector(); };
    glm::vec3 getCamPosition() { return cam->getCameraPosition(); };
    //callbacks - for interaction
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow* window, double currX, double currY);
};