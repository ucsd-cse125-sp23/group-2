#include "core.h"
#include "Shader.h"
#include "stb_image.h"
#include "ObjectModel.h"
#include "../Prefabs.h"

class Skybox {

public:
	Skybox();
	void draw(const glm::mat4& viewProjMtx);
	void drawPortals(const glm::mat4& viewProjMtx, float time);
	unsigned int VAO;
	unsigned int texture;

	glm::vec3 position;

	glm::mat4 model;
	glm::vec3 color;
	Shader* skyShader;
	Shader* envShader;
	Shader* portalShader;
	ObjectModel* envModel;
	ObjectModel* domeModel;
	ObjectModel* portalModel;

	void setSkyShader(Shader* s) { skyShader = s; }
	void setEnvShader(Shader* s) { envShader = s; }
	void setPortalShader(Shader* s) { portalShader = s; }

private:
	unsigned int loadCubemap(std::vector<std::string> faces);

};