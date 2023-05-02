#include "core.h"
#include "Shader.h"
#include "stb_image.h"

class Skybox {

public:
	Skybox();
	void draw(const glm::mat4& viewProjMtx, Shader* shader);
	unsigned int VAO;
	unsigned int texture;

private:
	unsigned int loadCubemap(std::vector<std::string> faces);

};