#pragma once

#include <stdio.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "core.h"


class Shader
{
public:
	Shader();
	Shader(const char* vertex_path, const char* fragment_path);

	void use();
	void cleanup();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	void setFloat(const std::string& name, float value) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	unsigned int ID;
};
