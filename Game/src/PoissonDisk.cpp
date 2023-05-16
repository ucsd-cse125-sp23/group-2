#include "PoissonDisk.h"

std::vector<glm::vec3> PoissonDisk::genPoints()
{
	//Init pgrid
	std::fill(&pgrid[0][0], &pgrid[0][0] + sizeof(pgrid) / sizeof(pgrid[0][0]), -1);
	glm::vec3 init = glm::vec3((std::rand() / ((float)RAND_MAX)) * WORLD_X, 0, (std::rand() / ((float)RAND_MAX)) * WORLD_Z);

	std::vector<glm::vec3> active;

	std::vector<glm::vec3> final;

	active.push_back(init);
	pgrid[(int)(init.x / WORLD_X)][(int)(init.z / WORLD_Z)] = final.size();

	final.push_back(init);


	while (!active.empty()) {

		//Pop random point from active
		int r = (std::rand() / ((float)RAND_MAX)) * active.size();
		glm::vec3 curr = active.at(r);
		active[r] = active.back();
		active.pop_back();
		for (int i = 0; i < samples; ++i) {
			float rrad = radius* ((std::rand() / ((float)RAND_MAX)) + 1);
			float rangle = (std::rand() / ((float)RAND_MAX))*2.0*glm::pi<float>();
			glm::vec3 rpoint = glm::vec3(rrad * glm::cos(rangle), 0, rrad * glm::sin(rangle)) + curr;
			if (validPoint(rpoint, final)) {
				active.push_back(rpoint);
				pgrid[(int)(rpoint.x / WORLD_X)][(int)(rpoint.z / WORLD_Z)] = final.size();
				final.push_back(rpoint);
			}
		}
	}
	return final;
}

bool PoissonDisk::validPoint(glm::vec3& point, std::vector<glm::vec3> & final)
{
	if (point.x > WORLD_X || point.z > WORLD_Z) {
		return false;
	}
	int xpos = (int)(point.x / WORLD_X);
	int zpos = (int)(point.z / WORLD_X);
	for (int i = glm::max(xpos - 2, 0); i <= glm::min(xpos + 2,(int)gridx-1) ; ++i) {
		for (int j = glm::max(zpos - 2, 0); j <= glm::min(zpos + 2, (int)gridz - 1); ++j){
			if (i == xpos && j == zpos) {
				continue;
			}
			if (pgrid[i][j] != -1) {
				if (glm::distance(final.at(pgrid[i][j]), point) < radius) {
					return false;
				}
			}
		}
	}
	return true;
}
