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
std::list<Entity> createProjectileMartian();
std::list<Entity> createProjectileSpread5();
std::list<Entity> createProjectileChaos();
std::list<Entity> createProjectileRandom();
std::list<Entity> createProjectilePierce();
std::list<Entity> createProjectilePierce1();
std::list<Entity> createProjectilePierce2();
std::list<Entity> createProjectilePierce3();

std::list<Entity> createProjectileSpray();
std::list<Entity> createEnemyGroundBasic();
std::list<Entity> createEnemyGroundTank();
std::list<Entity> createEnemyGroundMini();
std::list<Entity> createEnemyFlyingBasic();
std::list<Entity> createEnemyFlyingTractor();
std::list<Entity> createTowerReticleBasic();
std::list<Entity> createTowerReticleRailgun();
std::list<Entity> createTowerReticleTesla();
std::list<Entity> createTowerReticleBarrier();

std::list<Entity> createTowerBasic();
std::list<Entity> createTowerBasic1();
std::list<Entity> createTowerBasic2();
std::list<Entity> createTowerBasic3();

std::list<Entity> createTowerRailgun();
std::list<Entity> createTowerRailgun1();
std::list<Entity> createTowerRailgun2();
std::list<Entity> createTowerRailgun3();

std::list<Entity> createTowerTesla();
std::list<Entity> createTowerTesla1();
std::list<Entity> createTowerTesla2();
std::list<Entity> createTowerTesla3();

std::list<Entity> createTowerBarrier();
std::list<Entity> createTowerBarrier1();
std::list<Entity> createTowerBarrier2();
std::list<Entity> createTowerBarrier3();

std::list<Entity> createEnemyGroundTank();
std::list<Entity> createEnemyFlyingBasic();
std::list<Entity> createHome();
std::list<Entity> createWoodResourceBasic();
std::list<Entity> createStoneResourceBasic();
std::list<Entity> createPathColliders();
std::list<Entity> createPlayers();

std::list<Entity> createBossHead();
std::list<Entity> createBossLArm();
std::list<Entity> createBossBody();
std::list<Entity> createBossRArm();
std::list<Entity> createBossRLeg();
std::list<Entity> createBossLLeg();
std::list<Entity> createBoss();

std::list<Entity> createPowerupRandom();
std::list<Entity> createBounds();



namespace Prefabs {
	enum Prefab : uint32_t{
		TowerBasic,
		TowerRailgun,
		TowerTesla,
		TowerBarrier,
		TowerBasic1,
		TowerRailgun1,
		TowerTesla1,
		TowerBarrier1,
		TowerBasic2,
		TowerRailgun2,
		TowerTesla2,
		TowerBarrier2,
		TowerBasic3,
		TowerRailgun3,
		TowerTesla3,
		TowerBarrier3,

		TOWER_MARKER,//Put Towers Above this marker
		ProjectileBasic = TOWER_MARKER,
		ProjectileMartian,
		ProjectileSpread5,
		ProjectileChaos,
		ProjectileRandom,
		ProjectilePierce,
		ProjectilePierce1,
		ProjectilePierce2,
		ProjectilePierce3,
		ProjectileSpray,


		PROJECTILE_MARKER,//Put Projectiles Above this marker
		EnemyGroundBasic = PROJECTILE_MARKER,
		EnemyGroundTank,
		EnemyGroundMini,
		EnemyFlyingBasic,
		EnemyFlyingTractor,

		ENEMY_MARKER,//Put Enemies Above this marker
		BASIC_WOOD_RESOURCE = ENEMY_MARKER,
		BASIC_STONE_RESOURCE,

		RESOURCE_MARKER, //Put Resource above this marker
		TowerReticleBasic = RESOURCE_MARKER,
		TowerReticleRailgun,
		TowerReticleTesla,
		TowerReticleBarrier,
		PathColliders,
		UI_MARKER,
		Home = UI_MARKER,
		EnemyBoss,
		Players,
		PowerupRandom,
		Bounds,

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
	&createTowerRailgun,
	&createTowerTesla,
	&createTowerBarrier,
	&createTowerBasic1,
	&createTowerRailgun1,
	&createTowerTesla1,
	&createTowerBarrier1,
	&createTowerBasic2,
	&createTowerRailgun2,
	&createTowerTesla2,
	&createTowerBarrier2,
	&createTowerBasic3,
	&createTowerRailgun3,
	&createTowerTesla3,
	&createTowerBarrier3,
	&createProjectileBasic,
	&createProjectileMartian,
	&createProjectileSpread5,
	&createProjectileChaos,
	&createProjectileRandom,
	&createProjectilePierce,
	&createProjectilePierce1,
	&createProjectilePierce2,
	&createProjectilePierce3,
	&createProjectileSpray,
	&createEnemyGroundBasic,
	&createEnemyGroundTank,
    &createEnemyGroundMini,
	&createEnemyFlyingBasic,
	&createEnemyFlyingTractor,
	&createWoodResourceBasic,
	&createStoneResourceBasic,
	&createTowerReticleBasic,
	&createTowerReticleRailgun,
	&createTowerReticleTesla,
	&createTowerReticleBarrier,
	&createPathColliders,
	&createHome,
	&createBoss,
	&createPlayers,
	&createPowerupRandom,
	&createBounds
};

namespace PlayerSpawns {
	extern constexpr int numSpawns = 4; //Should always be >= NUM_PLAYERS
	extern const glm::vec3 spawnpoint[numSpawns];
};

namespace Paths {
	extern constexpr int bossPath = 2;
	extern constexpr int pathCount = 5;
	extern const glm::vec3 path[pathCount][PATH_LENGTH];
	extern std::list<Entity> pathlist;
};

namespace Boundry {
	extern std::list<Entity> boundlist;
}

struct enemy {
	int id;
	int path;
	float cooldown;
};

namespace WaveData {

	extern int currentWave;

	extern int waveTimers[WAVE_COUNT]; //Time before starting wave

	extern int waveTick; //countdown timer for waves

	extern int enemyTypes[NUM_ENEMY_TYPES]; //For random enemy selection

	extern std::queue<enemy> waves[WAVE_COUNT];
};

//Define buildcosts
extern const std::array<std::array<int, NUM_RESOURCE_TYPES>, NUM_TOWER_TYPES> buildcosts;

//Define Base Location
const glm::vec3 baseLoc = glm::vec3(0, 0, 100);