#include "Mob.h"

Mob::Mob(int i) {
    // Model matrix.
    active = false;
    id = i;
    model = glm::mat4(1.0f);
    color = glm::vec3(0.0f, 0.0f, 1.0f);

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    ourShader = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

    // load models
    // -----------
    ourModel = new ObjectModel("../assets/cube.obj");

}

Mob::~Mob() {
    // Delete the VBOs and the VAO.

}

void Mob::draw(const glm::mat4& viewProjMtx, Shader* shader) {
    // actiavte the shader program
    shader->use();

    // get the locations and send the uniforms to the shader
    shader->setMat4("viewProj", viewProjMtx);
    shader->setMat4("model", model);

    ourModel->Draw(*shader);

    glUseProgram(0);
}

void Mob::update() {
    // Spin the cube
    spin(0.05f);
}
/*
void Mob::update(glm::vec3& translation) {
    model = model * glm::translate(translation);
}
*/
void Mob::update(glm::vec3& position) {
    model[3] = glm::vec4(position, 1.0f);
}

void Mob::spin(float deg) {
    // Update the model matrix by multiplying a rotation matrix
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}