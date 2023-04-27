#include "core.h"
#include "LoadingHelper.h"

class Skybox {

public:
	Skybox();
	void draw(const glm::mat4& viewProjMtx, Shader* shader);
	unsigned int VAO;
	unsigned int texture;

private:





};