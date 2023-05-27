#pragma once
#include <vector>
#include "core.h"

class Health {
private:
	float maxHealth;
	float currHealth;

    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;

    glm::mat4 model;
    glm::vec3 color;

    // Cube Information
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
public:
    Health();
	void update(glm::vec3 playerPos, glm::vec3 targetPos);
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
};