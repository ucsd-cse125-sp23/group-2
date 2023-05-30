#include "Cube.h"

Cube::Cube(glm::vec3 cubeMin, glm::vec3 cubeMax) {

    model = glm::mat4(1.0f);
    color = glm::vec3(0.0f, 0.2f, 0.8f);

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
    ourModel = new ObjectModel("../assets/cube/cube.obj");
}

Cube::~Cube() {
    // Delete the VBOs and the VAO.

}

void Cube::draw(const glm::mat4& viewProjMtx) {
    // actiavte the shader program
    ourShader->use();

    // get the locations and send the uniforms to the shader
    ourShader->setMat4("viewProj", viewProjMtx);
    ourShader->setMat4("model", model);

    ourModel->Draw(*ourShader);

    glUseProgram(0);
}

void Cube::update(glm::vec3& pos, glm::vec3& aabb) {
    model = glm::scale(aabb);
    model[3] = glm::vec4(pos, 1.0f);
}

