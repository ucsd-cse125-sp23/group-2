#pragma once
#include <vector>
#include "core.h"
#include "Shader.h"
#include "ObjectModel.h"
class Mob {
private:

    bool active;
    int id;
    glm::vec3 color;
    glm::mat4 model;

    Shader* ourShader;
    ObjectModel* ourModel;

public:
    Mob(int i);
    ~Mob();

    void draw(const glm::mat4& viewProjMtx, Shader* shader);
    void update();
    void update(glm::vec3& translation);

    void spin(float deg);
    void setActive(bool a) { active = a; };
    bool getActive() { return active; };
};