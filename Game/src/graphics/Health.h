#pragma once
#include <vector>
#include "core.h"
#include "Shader.h"
#include "ObjectModel.h"

class HealthBar {
private:
	float maxHealth;
	float currHealth;
    bool active;
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;

    glm::mat4 model;
    glm::vec3 color;

    // Cube Information
    Shader* ourShader;
    ObjectModel* ourModel;
public:
    HealthBar(Shader * s, ObjectModel * model);
	void update(glm::vec3 playerPos, glm::vec3 targetPos, float cH, float mH);
	void draw(const glm::mat4& viewProjMtx);
    void setActive(bool a) { active = a; };
    bool getActive() { return active; };
    float getCurrHealth() { return currHealth; }

};