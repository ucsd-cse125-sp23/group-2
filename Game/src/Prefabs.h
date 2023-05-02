#pragma once
#include <list>
#include "EntityComponentSystem.h"

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

std::list<Entity> createProjectileBasic();
std::list<Entity> createProjectileSpread5();

const int NUM_PREFABS = 2;
using Prefab = uint32_t;
typedef std::list<Entity>(*PrefabFunction)(); // function pointer type
const PrefabFunction prefabMap[NUM_PREFABS] = {
	&createProjectileBasic,
	&createProjectileSpread5};

namespace Prefabs {
	constexpr Prefab ProjectileBasic = 0;
	constexpr Prefab ProjectileSpread5 = 1;
};

