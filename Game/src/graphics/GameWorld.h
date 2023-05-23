#pragma once
#include "Player.h"
#include "Mob.h"
#include "Skybox.h"
#include "Shader.h"
#include "Camera.h"
#include "gui/GUIElement.h"
#include "../GameConstants.h"
#include "../NetworkData.h"
class GameWorld {

private:
    int currID;
    std::array <Player*, NUM_CLIENTS> players;
    std::array <Mob*, NUM_ENEMIES> mobs;
   
    Skybox* env;
    Camera* cam;
    static float prevX, prevY, currX, currY, scrollY;



    //static int mouseDX, mouseDY;

public:
    void init();
    
    int width;
    int height;
    //Buttons
    GUIElement* continue_Btn;
    GUIElement* options_Btn;
    GUIElement* quit_Btn;
    GUIElement* back_Btn;

    //Volume
    GUIElement* volume_Sldr;
    GUIElement* volume_Sldr_Back;
    GUIElement* volume_Sldr_Front;

    //Health
    GUIElement* health_Bar;
    GUIElement* health_Bar_Back;
    GUIElement* health_Bar_Front;
    //
    void GUI_Init();
    void update(ServertoClientData& incomingData, int id);

    //render all active entities
    void draw(Shader* shader, Shader* skyboxShader, Shader* guiShader, float width, float height);

    float getCamAngle() { return cam->getAngleAroundPlayer(); };
    glm::vec3 getCamDirectionVector() { return cam->getDirectionVector(); };
    glm::vec3 getCamPosition() { return cam->getCameraPosition(); };
    //callbacks - for interaction
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow* window, double currX, double currY);
};