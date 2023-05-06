#pragma once
#include <list>
#include "graphics/core.h"
#include "GameConstants.h"

/*
* The default axis is negative z is the towards the target
* Positive x is right, negative x is left
* Positve y is up
* negative y is down
* Unit scale is the same
* Originates at the origin
* (Must transform, positions and velocities to make use of prefabs)
*
*/
using Entity = uint32_t;

std::list<Entity> createProjectileBasic();
std::list<Entity> createProjectileSpread5();
std::list<Entity> createEnemyGroundBasic();

const int NUM_PREFABS = 3;
using Prefab = uint32_t;
typedef std::list<Entity>(*PrefabFunction)(); // function pointer type
const PrefabFunction prefabMap[NUM_PREFABS] = {
	&createProjectileBasic,
	&createProjectileSpread5,
	&createEnemyGroundBasic};

namespace Prefabs {
	constexpr Prefab ProjectileBasic = 0;
	constexpr Prefab ProjectileSpread5 = 1;
	constexpr Prefab EnemyGroundBasic = 2;
};

namespace Paths {
	extern int const pathCount;
	extern glm::vec3 path[4][PATH_LENGTH];
};

#define PREF_ENEMY_GROUND_BASIC 2
