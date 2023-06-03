#pragma once
#include <vector>
#include "core.h"
#include "Shader.h"
#include "ObjectModel.h"
class Cube {
private:
    bool active;
    glm::mat4 model;
    glm::vec3 color;

    Shader* ourShader;
    ObjectModel* ourModel;

public:
    Cube(Shader * s, ObjectModel * m);
    ~Cube();

    void draw(const glm::mat4& viewProjMtx);
    void update(glm::vec3 & pos, glm::vec3 & aabb);
    void setActive(bool a) { active = a; };
    bool getActive() { return active; };
    void spin(float deg);
};