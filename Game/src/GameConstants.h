//The max number of each entity type (CAN BE MODIFIED - Currently Arbitrary)
#pragma once
#define NUM_PLAYERS 4
#define NUM_ENEMIES 20
#define NUM_TOWERS 20
#define NUM_RESOURCES 20
#define NUM_PROJECTILES 50
const float TICK_RATE = 128;

//Speed of movement in units per second
const float PLAYER_MVSPD_PERSEC = 20;

//Base health values of different game entities
const float PLAYER_BASE_HEALTH = 119;
const float ENEMY_BASE_HEALTH = 100;

//Base damage per second dealt by towers
const float TURRET_BASE_DPS = 200;

//ground enemy base movespeed in units/sec
const float ENEMY_GND_MVSPD_PERSEC = 5;

const float ENEMY_SPAWNDELAY_SEC = 1;

//ground enemy base attack damage
const float ENEMEY_GND_BASE_DMG = 30;

//Length of LONGEST path in-game (Final value will be determiend by world shape)
#define PATH_LENGTH 8

//Model ID's (For Cory/Will to decide)
#define MODEL_ID_ROVER 0
//.... ADD MORE ...


////////////////////////////////////////////////////////////////////////////
// **************** DO NOT MODIFY VALUES BELOW THIS LINE **************** //
////////////////////////////////////////////////////////////////////////////

//Values per second adjusted for tick rate
const float ENEMY_GND_BASE_MVSPD = (ENEMY_GND_MVSPD_PERSEC / TICK_RATE);
const float TURRET_BASE_DMG = (TURRET_BASE_DPS / TICK_RATE);
const float PLAYER_MVSPD = (PLAYER_MVSPD_PERSEC / TICK_RATE);
const float ENEMY_SPAWNDELAY_TICKS = (ENEMY_SPAWNDELAY_SEC * TICK_RATE);

//The Start and End indices of the entity array for each entity type 
#define ENEMY_START         (NUM_PLAYERS)
#define ENEMY_END           (NUM_PLAYERS + NUM_ENEMIES)
#define TOWER_START         (NUM_PLAYERS + NUM_ENEMIES)
#define TOWER_END           (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS)
#define RESOURCE_START      (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS)
#define RESOURCE_END        (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES)
#define PROJECTILE_START    (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES)
#define PROJECTILE_END      (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES + NUM_PROJECTILES)

//Total number of entities
#define MAX_ENTITIES (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES + NUM_PROJECTILES)