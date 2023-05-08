#pragma once

#include "../core.h"
#include "../Shader.h"
#include "../stb_image.h"


class GUIElement
{
private:
	unsigned int VAO;
	unsigned int texture;

	bool isHidden;
	glm::vec2 position;
	glm::vec2 size;
	float transparency;
	char* texturePath;
	char* name;

public:
	GUIElement(glm::vec2 position = glm::vec2(0.0, 0.0), glm::vec2 size = glm::vec2(1.0, 1.0), float transparency = 0.5, char* texturePath = "../assets/container.jpg", char* name = "button", bool isHidden = false);
	
	void SetHidden(bool hide);
	void SetTexture(char* path);
	void SetSize(glm::vec2 newSize);
	void SetPosition(glm::vec2 newPosition);
	void SetTransparency(float newTransparency);
	void SetName(char* newName);

	bool IsHidden();

	void draw(const glm::mat4& viewProjMtx, Shader* shader);


};