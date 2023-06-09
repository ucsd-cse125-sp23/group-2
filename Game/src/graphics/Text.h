#include <ft2build.h>
#include FT_FREETYPE_H

#include "../GameConstants.h"
#include "core.h"
#include "Shader.h"
#include <map>

struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class Text {
private:
	std::map<GLchar, Character> Characters;
	unsigned int VAO, VBO;
	Shader* shader;
	std::string font;

public:

	Text(std::string font_name);
	void setShader(Shader* s) { shader = s; }
	glm::vec2 RenderText(std::string text, float x, float y, float scale, glm::vec3 color);


};