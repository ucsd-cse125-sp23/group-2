#pragma once
#include <vector>
#include "core.h"
class Player {
private:

    bool active;
    int id;

    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;

    glm::mat4 model;
    glm::vec3 color;

    // Cube Information
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

public:
    Player(int i, glm::vec3 cubeMin = glm::vec3(-0.2, -0.2, -0.2), glm::vec3 cubeMax = glm::vec3(0.2, 0.2, 0.2));
    ~Player();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update();
    void update(glm::vec3 & translation);

    void spin(float deg);
    void setActive(bool a) { active = a; };
    bool getActive() { return active; };
    
};