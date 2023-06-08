#include "Skybox.h"

Skybox::Skybox() {

	float vertices[] = {
		// positions          
		-1000.0f,  1000.0f, -1000.0f,
		-1000.0f, -1000.0f, -1000.0f,
		 1000.0f, -1000.0f, -1000.0f,
		 1000.0f, -1000.0f, -1000.0f,
		 1000.0f,  1000.0f, -1000.0f,
		-1000.0f,  1000.0f, -1000.0f,

		-1000.0f, -1000.0f,  1000.0f,
		-1000.0f, -1000.0f, -1000.0f,
		-1000.0f,  1000.0f, -1000.0f,
		-1000.0f,  1000.0f, -1000.0f,
		-1000.0f,  1000.0f,  1000.0f,
		-1000.0f, -1000.0f,  1000.0f,

		 1000.0f, -1000.0f, -1000.0f,
		 1000.0f, -1000.0f,  1000.0f,
		 1000.0f,  1000.0f,  1000.0f,
		 1000.0f,  1000.0f,  1000.0f,
		 1000.0f,  1000.0f, -1000.0f,
		 1000.0f, -1000.0f, -1000.0f,

		-1000.0f, -1000.0f,  1000.0f,
		-1000.0f,  1000.0f,  1000.0f,
		 1000.0f,  1000.0f,  1000.0f,
		 1000.0f,  1000.0f,  1000.0f,
		 1000.0f, -1000.0f,  1000.0f,
		-1000.0f, -1000.0f,  1000.0f,

		-1000.0f,  1000.0f, -1000.0f,
		 1000.0f,  1000.0f, -1000.0f,
		 1000.0f,  1000.0f,  1000.0f,
		 1000.0f,  1000.0f,  1000.0f,
		-1000.0f,  1000.0f,  1000.0f,
		-1000.0f,  1000.0f, -1000.0f,

		-1000.0f, -1000.0f, -1000.0f,
		-1000.0f, -1000.0f,  1000.0f,
		 1000.0f, -1000.0f, -1000.0f,
		 1000.0f, -1000.0f, -1000.0f,
		-1000.0f, -1000.0f,  1000.0f,
		 1000.0f, -1000.0f,  1000.0f
	};

	std::vector<std::string> faces =
	{
		"../assets/skybox/right.png",
		"../assets/skybox/left.png",
		"../assets/skybox/top.png",
		"../assets/skybox/bottom.png",
		"../assets/skybox/front.png",
		"../assets/skybox/back.png"
	};

	texture = loadCubemap(faces);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	model = glm::mat4(1.0f);
	model[3] = glm::vec4(0.0f, 39.0f, 0.0f, 1.0f);

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(false);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	envModel = new ObjectModel("../assets/environment/environment.obj");
	domeModel = new ObjectModel("../assets/dome/dome.obj");
}

void Skybox::draw(const glm::mat4& viewProjMtx) {
	glDepthMask(GL_FALSE);
	skyShader->use();
	skyShader->setMat4("viewProjMtx", viewProjMtx);
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);


	envShader->use();
	// get the locations and send the uniforms to the shader
	envShader->setMat4("viewProj", viewProjMtx);
	envShader->setMat4("model", model);
	envModel->Draw(*envShader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	domeModel->Draw(*envShader);
	glDisable(GL_BLEND);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int Skybox::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}