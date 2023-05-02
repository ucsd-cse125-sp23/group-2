#include "Player.h"

Player::Player(int i) {
    // Model matrix.
    active = false;
    id = i;
    model = glm::mat4(1.0f);
<<<<<<< Updated upstream

    // The color of the cube. Try setting it to something else!
    color = glm::vec3(0.0f, 0.0f, 1.0f);
    bool res = loadOBJ("../assets/cube.obj", indices, vertices, uvs, normals);

    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glGenBuffers(1, &VBO_positions);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glGenBuffers(1, &VBO_normals);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glGenBuffers(1, &VBO_uvs);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uvs);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

=======
>>>>>>> Stashed changes
    
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
void Player::update(glm::vec3& position) {
    this->position = position;
    model[3] = glm::vec4(position, 1.0f);
}

void Player::spin(float deg) {
    // Update the model matrix by multiplying a rotation matrix
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

