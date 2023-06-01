#include "core.h"
#include "Shader.h"
#include "stb_image.h"
#include "ObjectModel.h"

class Skybox {

public:
	Skybox();
	void draw(const glm::mat4& viewProjMtx);
	unsigned int VAO;
	unsigned int texture;

	glm::vec3 position;

	glm::mat4 model;
	glm::vec3 color;
	Shader* skyShader;
	Shader* envShader;
	ObjectModel* envModel;
	ObjectModel* domeModel;

	void setSkyShader(Shader* s) { skyShader = s; }
	void setEnvShader(Shader* s) { envShader = s; }

private:
	unsigned int loadCubemap(std::vector<std::string> faces);

};