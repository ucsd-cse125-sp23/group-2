#pragma once
#include "graphics/core.h"
#include "GameConstants.h"
#include <vector>
#include <list>
#include <tuple>

namespace PoissonDisk {
	constexpr float radius = 5;
	constexpr float samples = 30;
	constexpr float side = (radius / 1.41421356237);
	static constexpr size_t gridx = WORLD_X / side;
	static constexpr size_t gridz = WORLD_Z / side;
	int pgrid[gridx][gridz];
	std::vector<glm::vec3> genPoints();
	bool validPoint(glm::vec3 & point, std::vector<glm::vec3> & final);
}