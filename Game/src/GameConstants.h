//The max number of each entity type (CAN BE MODIFIED - Currently Arbitrary)
#pragma once
#define NUM_PLAYERS 4
#define NUM_ENEMIES 40
#define NUM_TOWERS 40
#define NUM_RESOURCES 100
#define NUM_PROJECTILES 100
#define NUM_BASES 1
#define CLOG_MAXSIZE 50
const float TICK_RATE = 128;

//World Length and Width
constexpr float WORLD_X = 164;
constexpr float WORLD_Z = 164;

//Speed of movement in units per second
const float PLAYER_MVSPD_PERSEC = 20;

//Initial jump speed
const float PLAYER_JPSPD_PERSEC = 30;

//Base health values of different game entities
const float PLAYER_BASE_HEALTH = 119;
const float ENEMY_BASE_HEALTH = 100;
const float HOME_BASE_HEALTH = 100;
const float RESOURCE_BASE_HEALTH = 100;

//Base damage per second dealt by towers
const float TURRET_BASE_DPS = 200;

//ground enemy base movespeed in units/sec
const float ENEMY_GND_MVSPD_PERSEC = 5;

//How many seconds until first wave spawns
const float ENEMY_SPAWNDELAY_SEC = 1;

//ground enemy base attack damage
const float ENEMY_GND_BASE_DMG = 30;

//how close players must be to gain the attention of enemies
const float AGRO_RANGE = 10;

//how far players must be for enemies to lose interest in them
const float DEAGRO_RANGE = 20;

//gravitational acceleration in units per second squared
const float GRAVITY_SEC = -120;

//How high above the ground flying enemies will hover
const float FLYING_HEIGHT = 10;

//Length of LONGEST path in-game (Final value will be determiend by world shape)
#define PATH_LENGTH 8

#define WAVE_COUNT 5

#define NUM_ENEMY_TYPES 3

#define NUM_RESOURCE_TYPES 3

const float PATH_WIDTH = 3;

//TOwer placement range
const float TOWER_PLACEMENT_RANGE = 15;

//Model ID's (For Cory/Will to decide)
#define MODEL_ID_NO_MODEL -1
#define MODEL_ID_CUBE 0
#define MODEL_ID_ROVER 1
#define MODEL_ID_MOB 2
#define MODEL_ID_MOB_FLYING 3
#define MODEL_ID_TOWER 4
#define MODEL_ID_RESOURCE 5
#define MODEL_ID_PROJECTILE 6
#define MODEL_ID_BASE 7

#define NUM_MODELS 8

//animation states
#define ANIM_IDLE 0
#define ANIM_ATTACKING 1
#define ANIM_TAKING_DAMAGE 2
#define ANIM_DEATH 3

#define RES_WIDTH 1920
#define RES_HEIGHT 1080
//.... ADD MORE ...


////////////////////////////////////////////////////////////////////////////
// **************** DO NOT MODIFY VALUES BELOW THIS LINE **************** //
////////////////////////////////////////////////////////////////////////////

//Values per second adjusted for tick rate
const float ENEMY_GND_BASE_MVSPD = (ENEMY_GND_MVSPD_PERSEC / TICK_RATE);
const float TURRET_BASE_DMG = (TURRET_BASE_DPS / TICK_RATE);
const float PLAYER_MVSPD = (PLAYER_MVSPD_PERSEC / TICK_RATE);
const float PLAYER_JPSPD = (PLAYER_JPSPD_PERSEC / TICK_RATE);
const float ENEMY_SPAWNDELAY_TICKS = (ENEMY_SPAWNDELAY_SEC * TICK_RATE);
const float GRAVITY = (GRAVITY_SEC / (TICK_RATE * TICK_RATE));

//The Start and End indices of the entity array for each entity type
#define ENEMY_START         (NUM_PLAYERS)
#define ENEMY_END           (NUM_PLAYERS + NUM_ENEMIES)
#define TOWER_START         (NUM_PLAYERS + NUM_ENEMIES)
#define TOWER_END           (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS)
#define RESOURCE_START      (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS)
#define RESOURCE_END        (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES)
#define PROJECTILE_START    (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES)
#define PROJECTILE_END      (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES + NUM_PROJECTILES)
#define BASE_START			(NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES + NUM_PROJECTILES)
#define BASE_END			(NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES + NUM_PROJECTILES + NUM_BASES)

//Total number of entities
#define MAX_ENTITIES (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES + NUM_PROJECTILES + NUM_BASES)
