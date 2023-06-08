#pragma once
#include <vector>
#include "core.h"
#include "Camera.h"
#include "Cube.h"
#include "Shader.h"
#include "ObjectModel.h"
#include <random>
class RenderEntity {
private:
    bool active;
    int id;
    glm::vec3 position;
    float orientation;
    float offset;
    glm::mat4 model;
    glm::vec3 color;
    Shader* shader;
    ObjectModel* ourModel;
   

public:
    RenderEntity(int i);
    ~RenderEntity();

    void draw(const glm::mat4& viewProjMtx, float time, Camera* cam);
    void update(glm::vec3& translation, float degrees, float scale);

    void setActive(bool a) { active = a; };
    bool getActive() { return active; };

    void setShader(Shader* s) { shader = s; }
    void setModel(ObjectModel* m) { ourModel = m; }

    glm::vec3 getPosition() { return position; };
};