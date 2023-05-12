#pragma once
#include <vector>
#include "core.h"
#include "Shader.h"
#include "ObjectModel.h"
class Projectile {
private:

    bool active;
    int id;
    glm::vec3 position;
    float orientation;


    glm::mat4 model;
    glm::vec3 color;

    Shader* ourShader;
    ObjectModel* ourModel;

public:
    Projectile(int i);
    ~Projectile();

    void draw(const glm::mat4& viewProjMtx);
  
    void update(glm::vec3& translation, float degrees);

    void setActive(bool a) { active = a; };
    bool getActive() { return active; };
    glm::vec3 getPosition() { return position; };


};