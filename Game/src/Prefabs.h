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

		
		TOWER_MARKER,//Put Towers Above this marker
		ProjectileBasic = TOWER_MARKER,
		ProjectileSpread5,
		ProjectileChaos,
		ProjectileRandom,
		TowerReticle,

		
		PROJECTILE_MARKER,//Put Projectiles Above this marker
		EnemyGroundBasic = PROJECTILE_MARKER,
	
		ENEMY_MARKER,//Put Enemies Above this marker
		BASIC_WOOD_RESOURCE = ENEMY_MARKER,
		BASIC_STONE_RESOURCE,

		RESOURCE_MARKER, //Put Resource above this marker
	};
}

const int NUM_PREFAB = Prefabs::RESOURCE_MARKER;
const int NUM_TOWER_PREFAB = Prefabs::TOWER_MARKER;
const int NUM_PROJECTILE_PREFAB = Prefabs::PROJECTILE_MARKER - Prefabs::TOWER_MARKER;
const int NUM_ENEMY_PREFAB = Prefabs::ENEMY_MARKER - Prefabs::PROJECTILE_MARKER;

using namespace Prefabs;

typedef std::list<Entity>(*PrefabFunction)(); // function pointer type

//Add functions in same order as prefabs above
const PrefabFunction prefabMap[NUM_PREFAB] = {
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