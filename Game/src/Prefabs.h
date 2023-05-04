#pragma once
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

#include <vector>
#include <queue>
#include <array>
#include "EntityComponentSystem.h"
#include "GameConstants.h"

class Prefabs
{
public:

	void enemy_ground_basic(Entity e);
};