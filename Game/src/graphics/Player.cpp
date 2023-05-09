#include "Player.h"

glm::mat4 * Player::modelMatrices = new glm::mat4[NUM_PLAYERS];
Shader* Player::ourShader;
ObjectModel* Player::ourModel;
unsigned int Player::amount = NUM_PLAYERS;
unsigned int Player::buffer;

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

    if(!ourShader)
        ourShader = new Shader("../shaders/instanceShader.vert", "../shaders/instanceShader.frag");
    if(!ourModel)
        ourModel = new ObjectModel("../assets/rover/rover.obj");
    
}

Player::~Player() {
    
}

void Player::setupMesh() {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    // set transformation matrices as an instance vertex attribute (with divisor 1)
    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
    // -----------------------------------------------------------------------------------------------------------------------------------
    for (unsigned int i = 0; i < ourModel->meshes.size(); i++)
    {
        unsigned int VAO = ourModel->meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

void Player::draw(const glm::mat4& viewProjMtx) {
    ourShader->use();
    ourShader->setMat4("viewProj", viewProjMtx);
    for (unsigned int i = 0; i < ourModel->meshes.size(); i++)
    {
        glBindVertexArray(ourModel->meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(ourModel->meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }
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
    model *= glm::rotate(glm::radians(orientationDiff), glm::vec3(0.0f, 1.0f, 0.0f));
    model[3] = glm::vec4(position, 1.0f);
    modelMatrices[id] = model;
}

void Player::spin(float deg) {
    // Update the model matrix by multiplying a rotation matrix
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

