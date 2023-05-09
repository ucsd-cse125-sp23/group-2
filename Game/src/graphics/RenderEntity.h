#pragma once
#include <vector>
#include "core.h"
#include "../GameConstants.h"
#include "Shader.h"
#include "ObjectModel.h"
class RenderEntity {
private:
    static glm::mat4* modelMatrices;
    static unsigned int buffer;
    static Shader* ourShader;
    static ObjectModel* ourModel;
    static unsigned int amount;

    bool active;
    int id;
    glm::vec3 position;
    float orientation;

    glm::mat4 model;
    glm::vec3 color;

public:
    RenderEntity(int i);
    ~RenderEntity();

    static void setupMesh();
    static void draw(const glm::mat4& viewProjMtx);
    void update(glm::vec3& translation, float degrees);

    void setActive(bool a) { active = a; };
    bool getActive() { return active; };
    glm::vec3 getPosition() { return position; };
    float getYRotation();

};