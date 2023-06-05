//The max number of each entity type (CAN BE MODIFIED - Currently Arbitrary)
#pragma once
#define NUM_PLAYERS 1
#define NUM_ENEMIES 50
#define NUM_TOWERS 50
#define NUM_RESOURCES 750
#define NUM_PROJECTILES 100
#define NUM_BASES 1
#define CLOG_MAXSIZE 50
#define SLOG_MAXSIZE 100
const float TICK_RATE = 64;

//World Length and Width
constexpr float WORLD_X = 150;
constexpr float WORLD_Z = 180;

//Speed of movement in units per second
const float PLAYER_MVSPD_PERSEC = 20;

//Initial jump speed
const float PLAYER_JPSPD_PERSEC = 30;

//Projectile Speed
const float PROJ_MVSPD_PERSEC = 64;

//How often projectiles spawn (seconds between spawns)
const float PROJ_SPAWN_RATE = 0.25;

//How often projectiles damage on collision (seconds between damages)
const float PROJ_DAMAGE_RATE = 0.25;

//Ground Height
const float GROUND_HEIGHT = -1.0;

//Base health values of different game entities
const float PLAYER_BASE_HEALTH = 100;
const float ENEMY_BASE_HEALTH = 200;
const float HOME_BASE_HEALTH = 1000;
const float RESOURCE_BASE_HEALTH = 200;
const float BARRIER_BASE_HEALTH = 100;

//Base damage per second dealt by towers
const float TURRET_BASE_DPS = 400;

//How often turrets fire (seconds between damages)
const float TURRET_BASE_FIRE_RATE = 0.25;

//How often turrets fire (seconds between damages)
const float TURRET_BASE_RANGE = 10;

//Snap range barrier
const float SNAP_RANGE = 5;

//Number of levels for of upgrades
constexpr size_t NUM_UP_LEVELS = 3;

//Player build cooldown in ticks
const int ACTION_COOLDOWN = TICK_RATE;

//ground enemy base movespeed in units/sec
const float ENEMY_GND_MVSPD_PERSEC = 5;

//How many seconds until first wave spawns
const float ENEMY_SPAWNDELAY_SEC = 60;

//ground enemy base attack damage
const float ENEMY_GND_BASE_DMG = 30;

//How often enemies damage on collision (seconds between damages)
const float ENEMY_BASE_DAMAGE_RATE = 0.25;

//how close players must be to gain the attention of enemies
const float AGRO_RANGE = 10;

//how far players must be for enemies to lose interest in them
const float DEAGRO_RANGE = 20;

//how close players must be for enemies to shoot at them
const float ATTACK_RANGE = 20;

//how close players must be to start being abducted
const float ABDUCT_RANGE = 15;

//how long abductions take
const float ABDUCT_TIMER = 1.0f;

//gravitational acceleration in units per second squared
const float GRAVITY_SEC = -120;

//How high above the ground flying enemies will hover
const float FLYING_HEIGHT = 10;

//Default player spawn timer in seconds
const float RESPAWN_TIMER = 5.0;

//Boss Stomp Rate
const float STOMP_RATE = 2;

//Stomp dmg
const float STOMP_DMG = 50;

//Stomp range
const float STOMP_RANGE = 5;

//Player safe area
const float SAFE_AREA = 80;

//Length of LONGEST path in-game (Final value will be determiend by world shape)
#define PATH_LENGTH 30

#define WAVE_COUNT 7

#define NUM_ENEMY_TYPES 5

#define NUM_TOWER_TYPES 4

#define NUM_RESOURCE_TYPES 3

const float PATH_WIDTH = 10;

//TOwer placement range
const float TOWER_PLACEMENT_RANGE = 15;

//Model ID's (For Cory/Will to decide)
#define MODEL_ID_NO_MODEL -1
#define MODEL_ID_CUBE 0
#define MODEL_ID_ROVER 1

#define MODEL_ID_MOB 2
#define MODEL_ID_MOB_TANK 3
#define MODEL_ID_MOB_MINI 4
#define MODEL_ID_MOB_FLYING 5
#define MODEL_ID_MOB_TRACTOR 6

#define MODEL_ID_TOWER 7
#define MODEL_ID_RAILGUN 8
#define MODEL_ID_TESLA 9
#define MODEL_ID_BARRIER 10

#define MODEL_ID_RESOURCE 11
#define MODEL_ID_RESOURCE_STONE 12

#define MODEL_ID_PROJECTILE 13

#define MODEL_ID_BASE 14
#define MODEL_ID_BEAR 15
#define MODEL_ID_SUNGOD 16
#define MODEL_ID_BEAR_BODY 17
#define MODEL_ID_BEAR_HEAD 18
#define MODEL_ID_BEAR_LARM 19
#define MODEL_ID_BEAR_RARM 20
#define MODEL_ID_BEAR_LLEG 21
#define MODEL_ID_BEAR_RLEG 22




#define NUM_MODELS 23

//animation states
#define ANIM_IDLE 0
#define ANIM_ATTACKING 1
#define ANIM_TAKING_DAMAGE 2
#define ANIM_DEATH 3

#define RES_WIDTH 1920
#define RES_HEIGHT 1080

//Sound IDs
#define SOUND_ID_IDLE 0
#define SOUND_ID_ATTACK 1
#define SOUND_ID_DAMAGE 2
#define SOUND_ID_DEATH 3
#define SOUND_ID_LAND 4
#define SOUND_ID_BUILD 5
#define SOUND_ID_JUMP 6

#define NUM_SOUND_TYPES 7

#define INVALID_CLIENT_ID NUM_PLAYERS
#define UNKNOWN_SERVER_STATUS -1
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
const float PROJ_MVSPD = PROJ_MVSPD_PERSEC/TICK_RATE;

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
#define MAX_ENTITIES_NOBASE (NUM_PLAYERS + NUM_ENEMIES + NUM_TOWERS + NUM_RESOURCES + NUM_PROJECTILES + NUM_BASES - 1)

