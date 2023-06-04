#include "Health.h"

HealthBar::HealthBar(Shader* s, ObjectModel* m) {
    model = glm::mat4(1.0f);

    color = glm::vec3(0.0f, 1.0f, 0.0f);

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    ourShader = s;

    // load models
    // -----------
    ourModel = m;
}

void HealthBar::draw(const glm::mat4& viewProjMtx) {
    // actiavte the shader program
    ourShader->use();

    // get the locations and send the uniforms to the shader
    ourShader->setMat4("viewProj", viewProjMtx);
    ourShader->setMat4("model", model);
    ourShader->setVec3("DiffuseColor", color);

    ourModel->Draw(*ourShader);

    glUseProgram(0);
}

void HealthBar::update(glm::vec3 playerPos, glm::vec3 targetPos, float cH, float mH) {
    //update HealthBar
    maxHealth = mH;
    currHealth = cH;

    //rotate to look at player
    glm::vec3 toVec = glm::normalize(playerPos - targetPos);
    float angle = glm::atan(toVec.z, toVec.x);
    model = glm::rotate(-angle, glm::vec3(0, 1, 0));
    model *= glm::scale(glm::vec3(0.01f, 0.2f, currHealth / maxHealth));
    //update location
    model[3] = glm::vec4(targetPos, 1.0f);
    model[3][1] += 1;  

}