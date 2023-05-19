#include "RenderEntity.h"

RenderEntity::RenderEntity(int i) {
    // Model matrix.
    active = false;
    id = i;
    orientation = 0;
    model = glm::mat4(1.0f);
    color = glm::vec3(0.0f, 0.2f, 0.8f);

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
}

RenderEntity::~RenderEntity() {

}

void RenderEntity::draw(const glm::mat4& viewProjMtx) {
    // actiavte the shader program
    shader->use();

    // get the locations and send the uniforms to the shader
    shader->setMat4("viewProj", viewProjMtx);
    shader->setMat4("model", model);

    ourModel->Draw(*shader);

    glUseProgram(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderEntity::update(glm::vec3& position, float deg) {
    this->position = position;
    model = glm::rotate(glm::radians(-deg), glm::vec3(0.0f, 1.0f, 0.0f));
    model[3] = glm::vec4(position, 1.0f);
}
