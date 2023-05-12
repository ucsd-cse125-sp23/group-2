#include "Projectile.h"

Projectile::Projectile(int i) {
    // Model matrix.
    active = false;
    id = i;
    orientation = 0;
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
    ourModel = new ObjectModel("../assets/cube.obj");


}

Projectile::~Projectile() {

}

void Projectile::draw(const glm::mat4& viewProjMtx) {
    // actiavte the shader program
    ourShader->use();

    // get the locations and send the uniforms to the shader
    ourShader->setMat4("viewProj", viewProjMtx);
    ourShader->setMat4("model", model);

    ourModel->Draw(*ourShader);

    glUseProgram(0);

}

void Projectile::update(glm::vec3& position, float deg) {
    this->position = position;
    float orientationDiff = orientation - deg;
    orientation = deg;
    /*
    float angle = orientation;
    if (v.x != 0) {
       angle = glm::atan(v.z / v.x);
    }
    float deltaAngle = angle - orientation;

    orientation = angle;
    model = model * glm::rotate(deltaAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    */
    model *= glm::rotate(glm::radians(orientationDiff), glm::vec3(0.0f, 1.0f, 0.0f));
    model[3] = glm::vec4(position, 1.0f);
}
