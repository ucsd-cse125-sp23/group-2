#include "Player.h"

Player::Player(int i) {
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
    ourModel = new ObjectModel("../assets/rover/rover.obj");

    
}

Player::~Player() {
    
}

void Player::draw(const glm::mat4& viewProjMtx, Shader* shader) {
    // actiavte the shader program
    ourShader->use();

    // get the locations and send the uniforms to the shader
    ourShader->setMat4("viewProj", viewProjMtx);
    ourShader->setMat4("model", model);

    ourModel->Draw(*ourShader);

    glUseProgram(0);

}

void Player::update() {
    // Spin the cube
    spin(0.05f);
}
/*
void Player::update(glm::vec3& translation) {
    model = model * glm::translate(translation);
}
*/
void Player::update(glm::vec3& position, float deg) {
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

void Player::spin(float deg) {
    // Update the model matrix by multiplying a rotation matrix
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

