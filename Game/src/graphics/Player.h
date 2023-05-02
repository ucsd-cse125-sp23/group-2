#pragma once
#include <vector>
#include "core.h"
#include "Shader.h"
#include "ObjectModel.h"
class Player {
private:

    bool active;
    int id;
    glm::vec3 position;
    glm::vec3 color;
    glm::mat4 model;

    Shader* ourShader;
    ObjectModel* ourModel;

public:
    Player(int i);
    ~Player();

    void draw(const glm::mat4& viewProjMtx, Shader* shader);
    void update();
    void update(glm::vec3 & translation);

    void spin(float deg);
    void setActive(bool a) { active = a; };
    bool getActive() { return active; };
    glm::vec3 getPosition() { return position; };
    float getYRotation();
    
};