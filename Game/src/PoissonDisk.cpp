#include "PoissonDisk.h"
namespace PoissonDisk {
	int pgrid[gridx][gridz];
}

std::vector<glm::vec3> PoissonDisk::genPoints()
{
	//Init pgrid
	//std::fill(&pgrid[0][0], &pgrid[0][0] + sizeof(pgrid) / sizeof(pgrid[0][0]), -1);
	for (int i = 0; i < gridx; ++i) {
		for (int j = 0; j < gridz; ++j) {
			pgrid[i][j] = -1;
		}
	}
	glm::vec3 init = glm::vec3((std::rand() / (RAND_MAX+1.0)) * WORLD_X, 0, ((std::rand() / (RAND_MAX + 1.0)) * (WORLD_Z - SAFE_AREA)));

	std::vector<glm::vec3> active;

	std::vector<glm::vec3> final;

	active.push_back(init);
	pgrid[(int)(init.x / side)][(int)(init.z / side)] = final.size();

	final.push_back(init);

	//printf("Beginnging Loop\n");
	while (!active.empty()) {
		//Pop random point from active
		int r = (std::rand() / (RAND_MAX + 1.0)) * active.size();
		//printf("Beginnging Loop Inner %d out of %d\n", r, active.size());
		glm::vec3 curr = active.at(r);
		active.at(r) = active.back();
		active.pop_back();

		for (int i = 0; i < samples; ++i) {
			float rrad = radius* ((std::rand() / (RAND_MAX + 1.0)) + 1);
			float rangle = (std::rand() / (RAND_MAX + 1.0))*2.0*glm::pi<float>();
			glm::vec3 rpoint = glm::vec3(rrad * glm::cos(rangle), 0, rrad * glm::sin(rangle)) + curr;
			if (validPoint(rpoint, final)) {
				/*
				if (pgrid[(int)(rpoint.x / side)][(int)(rpoint.z / side)] != -1) {
					printf("You bad %d\n", pgrid[(int)(rpoint.x / side)][(int)(rpoint.z / side)]);
				}
				*/				
				active.push_back(rpoint);
				pgrid[(int)(rpoint.x / side)][(int)(rpoint.z / side)] = final.size();
				final.push_back(rpoint);
			}
		}
		//printf("End Loop Inner\n");
	}

	
	//Grid Sanity
	for (int i = 0; i < gridx; ++i) {
		for (int j = 0; j < gridz; ++j) {
			if (pgrid[i][j] != -1) {
				int xpos = (int)(final.at(pgrid[i][j]).x / side);
				int zpos = (int)(final.at(pgrid[i][j]).z / side);
				if (xpos != i) {
					printf("WTFx %d, %d\n", xpos, i);
				}
				if (zpos != j) {
					printf("WTFz %d, %d\n", zpos, j);
				}
			}
		}
	}
	return final;
}

bool PoissonDisk::validPoint(glm::vec3& point, std::vector<glm::vec3> & final)
{
	if (point.x >= WORLD_X || point.z >= (WORLD_Z - SAFE_AREA) || point.x <= 0 || point.z <= 0) {
		return false;
	}
	int xpos = (int)(point.x / side);
	int zpos = (int)(point.z / side);
	for (int j = glm::max(zpos - 2, 0); j <= glm::min(zpos + 2, (int)gridz - 1); ++j) {
	for (int i = glm::max(xpos - 2, 0); i <= glm::min(xpos + 2,(int)gridx-1) ; ++i) {
		
			if (pgrid[i][j] != -1) {

				if (glm::distance(final.at(pgrid[i][j]), point) <= radius) {
					return false;
				}
				/*
				if (i == xpos && j == zpos) {
					printf("%f > %f\n", glm::distance(final.at(pgrid[i][j]), point), radius);
					printf("This shouldn;'t happen, i: %d, j:%d\n", i, j);
					printf("Pos new(%f, %f)\n", point.x, point.z);
					printf("Pos old(%f, %f) at %d\n", final.at(pgrid[i][j]).x, final.at(pgrid[i][j]).z, pgrid[i][j]);
					printf("Pos old should be at (%d, %d)\n", (int) (final.at(pgrid[i][j]).x / side), (int) (final.at(pgrid[i][j]).z / side));
				}
				*/
			}
		}
	}
	return true;
}
