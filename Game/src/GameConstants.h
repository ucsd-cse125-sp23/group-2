//The max number of each entity type (CAN BE MODIFIED - Currently Arbitrary)
#pragma once
#define NUM_PLAYERS 4
#define NUM_ENEMIES 20
#define NUM_TOWERS 20
#define NUM_RESOURCES 20
#define NUM_PROJECTILES 50
#define TICK_RATE 128
#define NUM_GUI 10

// Speed of movement in units per second
const float MOVE_SPEED = 20;

const float PLAYER_BASE_HEALTH = 119;
const float ENEMY_BASE_HEALTH = 100;
const float TURRET_BASE_DPS = 200;

//The Start and End indices of the entity array for each entity type (DO NOT MODIFY)
#define ENEMY_START         (NUM_PLAYERS)
#define ENEMY_END           (NUM_PLAYERS + NUM_ENEMIES)
#define TOWER_START         (NUM_PLAYERS + NUM_ENEMIES)
#define TOWER_END           (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS)
#define RESOURCE_START      (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS)
#define RESOURCE_END        (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES)
#define PROJECTILE_START    (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES)
#define PROJECTILE_END      (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES + NUM_PROJECTILES)

//Total number of entities (DO NOT MODIFY)
#define MAX_ENTITIES (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES + NUM_PROJECTILES)

//Model ID's (For Cory/Will to decide)
#define MODEL_ID_ROVER 0
//.... ADD MORE ...

//Length of LONGEST path in-game (Final value will be determiend by world shape)
#define PATH_LENGTH 8