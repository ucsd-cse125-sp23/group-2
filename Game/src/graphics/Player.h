#pragma once
#include <vector>
#include "core.h"
#include "../LoadingHelper.h"
class Player {
private:
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, VBO_uvs, EBO;

    glm::mat4 model;
    glm::vec3 color;

    // Cube Information
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

public:
    Player(glm::vec3 cubeMin = glm::vec3(-1, -1, -1), glm::vec3 cubeMax = glm::vec3(1, 1, 1));
    ~Player();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update();
    void update(glm::vec3 & translation);

    void spin(float deg);
};