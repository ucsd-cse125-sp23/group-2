#pragma once
#include <list>
#include <queue>
#include "graphics/core.h"
#include "GameConstants.h"
//Do not include EntityComponentSystem.h

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
std::list<Entity> createEnemyGroundTank();
std::list<Entity> createEnemyFlyingBasic();
std::list<Entity> createHome();
std::list<Entity> createWoodResourceBasic();
std::list<Entity> createStoneResourceBasic();
std::list<Entity> createPathColliders();

const int NUM_PREFABS = 9;
using Prefab = uint32_t;

namespace Prefabs {
	enum Prefab : uint32_t{
		TowerBasic,


		TOWER_MARKER,//Put Towers Above this marker
		ProjectileBasic = TOWER_MARKER,
		ProjectileSpread5,
		ProjectileChaos,
		ProjectileRandom,


		PROJECTILE_MARKER,//Put Projectiles Above this marker
		EnemyGroundBasic = PROJECTILE_MARKER,

		ENEMY_MARKER,//Put Enemies Above this marker
		BASIC_WOOD_RESOURCE = ENEMY_MARKER,
		BASIC_STONE_RESOURCE,

		RESOURCE_MARKER, //Put Resource above this marker
		TowerReticle = RESOURCE_MARKER,
		PathColliders,
		UI_MARKER,
		Home = UI_MARKER,
		NUM_PREFAB

	};
}

const int NUM_TOWER_PREFAB = Prefabs::TOWER_MARKER;
const int NUM_PROJECTILE_PREFAB = Prefabs::PROJECTILE_MARKER - Prefabs::TOWER_MARKER;
const int NUM_ENEMY_PREFAB = Prefabs::ENEMY_MARKER - Prefabs::PROJECTILE_MARKER;
const int NUM_RESOURCE_PREFAB = Prefabs::RESOURCE_MARKER - Prefabs::ENEMY_MARKER;
using namespace Prefabs;
typedef std::list<Entity>(*PrefabFunction)(); // function pointer type

//Add functions in same order as prefabs above
const PrefabFunction prefabMap[NUM_PREFAB] = {
	&createTowerBasic,
	&createProjectileBasic,
	&createProjectileSpread5,
	&createProjectileChaos,
	&createProjectileRandom,
	&createEnemyGroundBasic,
	&createWoodResourceBasic,
	&createStoneResourceBasic,
	&createTowerReticle,
	&createTowerBasic,
	&createEnemyGroundTank,
	&createEnemyFlyingBasic,
	&createPathColliders,
	&createHome
};

namespace Prefabs {
	constexpr Prefab ProjectileBasic = 0;
	constexpr Prefab ProjectileSpread5 = 1;
	constexpr Prefab ProjectileChaos = 2;
	constexpr Prefab ProjectileRandom = 3;
	constexpr Prefab EnemyGroundBasic = 4;
	constexpr Prefab TowerReticle = 5;
	constexpr Prefab TowerBasic = 6;
	constexpr Prefab EnemyGroundTank = 7;
	constexpr Prefab EnemyFlyingBasic = 8;
};

namespace Paths {
	extern constexpr int pathCount = 5;
	extern const glm::vec3 path[pathCount][PATH_LENGTH];
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

	extern int enemyTypes[NUM_ENEMY_TYPES]; //For random enemy selection

	extern std::queue<enemy> waves[WAVE_COUNT];
};

//Define buildcosts
extern const std::array<std::array<int, NUM_RESOURCE_TYPES>, NUM_TOWER_PREFAB> buildcosts;
