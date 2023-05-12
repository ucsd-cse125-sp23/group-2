#pragma once
#include <list>
#include <queue>
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
std::list<Entity> createProjectileChaos();
std::list<Entity> createProjectileRandom();
std::list<Entity> createEnemyGroundBasic();
std::list<Entity> createTowerReticle();
std::list<Entity> createTowerBasic();



namespace Prefabs {
	enum Prefab : uint32_t{
		TowerBasic,
		NUM_TOWER_PREFAB,


		ProjectileBasic = NUM_TOWER_PREFAB,
		ProjectileSpread5,
		ProjectileChaos,
		ProjectileRandom,
		TowerReticle,
		NUM_PROJECTILETOWER_PREFAB,



		EnemyGroundBasic = NUM_PROJECTILETOWER_PREFAB,
		NUM_PREFAB, 



		NUM_PROJECTILE_PREFAB = NUM_PROJECTILETOWER_PREFAB - NUM_TOWER_PREFAB,
		NUM_ENEMY_PREFAB = NUM_PREFAB - NUM_PROJECTILETOWER_PREFAB

	};
}


using namespace Prefabs;

typedef std::list<Entity>(*PrefabFunction)(); // function pointer type

//Add functions in same order as prefabs above
const PrefabFunction prefabMap[Prefabs::NUM_PREFAB] = {
	&createTowerBasic,
	&createProjectileBasic,
	&createProjectileSpread5,
	&createProjectileChaos,
	&createProjectileRandom,
	&createTowerReticle,
	&createEnemyGroundBasic,
};



namespace Paths {
	extern int const pathCount;
	extern glm::vec3 path[4][PATH_LENGTH];
};

struct enemy {
	int id;
	int path;
	int cooldown;
};

namespace WaveData {

	extern int currentWave;

	extern int waveTimers[WAVE_COUNT]; //Time before starting wave

	extern int waveTick; //countdown timer for waves
	
	extern std::queue<enemy> waves[WAVE_COUNT];
}

//Define buildcosts
extern std::array<std::array<int, NUM_RESOURCE_TYPES>, NUM_TOWER_PREFAB> buildcosts;