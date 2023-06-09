//The max number of each entity type (CAN BE MODIFIED - Currently Arbitrary)
#pragma once
#define NUM_PLAYERS 4
#define NUM_ENEMIES 64
#define NUM_TOWERS 64
#define NUM_RESOURCES 1024
#define NUM_PROJECTILES 1024
#define NUM_BASES 1
#define CLOG_MAXSIZE 128
#define SLOG_MAXSIZE 256
const float TICK_RATE = 64;
#define NUM_GUI 25

//How many seconds until first wave spawns
const float ENEMY_SPAWNDELAY_SEC = 30;

//What wave to start at (prep is -1)
const float START_WAVE = -1;

//World Length and Width
constexpr float WORLD_X = 200;
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
const float HOME_BASE_HEALTH = 4000;
const float RESOURCE_BASE_HEALTH = 200;
const float BARRIER_BASE_HEALTH = 400;

//Base damage per second dealt by towers
const float TURRET_BASE_DPS = 400;

//How often turrets fire (seconds between damages)
const float TURRET_BASE_FIRE_RATE = 0.25;

//How often turrets fire (seconds between damages)
const float TURRET_BASE_RANGE = 20;

//Snap range barrier
const float SNAP_RANGE = 8;

//Number of levels for of upgrades
constexpr size_t NUM_UP_LEVELS = 3;

//Player build cooldown in ticks
const int ACTION_COOLDOWN = TICK_RATE;

//ground enemy base movespeed in units/sec
const float ENEMY_GND_MVSPD_PERSEC = 10;

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

//Powerup spawn chance
const float POWERUP_CHANCE = 0.10f;

//Powerup duration
const float POWERUP_DURATION_SEC = 5;

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
enum MODEL_ID {
	MODEL_ID_CUBE,
	MODEL_ID_ROVER_RED,
	MODEL_ID_ROVER_BLUE,

	MODEL_ID_ROVER_GREEN,

	MODEL_ID_ROVER_PURPLE,

 MODEL_ID_MOB,
 MODEL_ID_MOB_TANK,
 MODEL_ID_MOB_MINI,
 MODEL_ID_MOB_FLYING,
 MODEL_ID_MOB_TRACTOR,
 MODEL_ID_TOWER,
 MODEL_ID_TOWER_L2,
 MODEL_ID_TOWER_L3,
 MODEL_ID_TOWER_L4,
 MODEL_ID_TOWER_INVALID,
 MODEL_ID_RAILGUN,
 MODEL_ID_RAILGUN_L2,
 MODEL_ID_RAILGUN_L3,
 MODEL_ID_RAILGUN_L4,
 MODEL_ID_RAILGUN_INVALID,
 MODEL_ID_TESLA,
 MODEL_ID_TESLA_L2,
 MODEL_ID_TESLA_L3,
 MODEL_ID_TESLA_L4,
 MODEL_ID_TESLA_INVALID,
 MODEL_ID_BARRIER,
 MODEL_ID_BARRIER_L2,
 MODEL_ID_BARRIER_L3,
 MODEL_ID_BARRIER_L4,
 MODEL_ID_BARRIER_INVALID,
 MODEL_ID_RESOURCE,
 MODEL_ID_RESOURCE_1,
 MODEL_ID_RESOURCE_2,
 MODEL_ID_RESOURCE_STONE,
 MODEL_ID_PROJECTILE_ROVER,
 MODEL_ID_PROJECTILE_MARTIAN,
 MODEL_ID_PROJECTILE_SPREAD,
 MODEL_ID_PROJECTILE_RAPID,
 MODEL_ID_BASE,
 MODEL_ID_BEAR,
 MODEL_ID_SUNGOD,
 MODEL_ID_BEAR_BODY,
 MODEL_ID_BEAR_HEAD,
 MODEL_ID_BEAR_LARM,
 MODEL_ID_BEAR_RARM,
 MODEL_ID_BEAR_LLEG,
 MODEL_ID_BEAR_RLEG,
 MODEL_ID_PATH_STRAIGHT,
 MODEL_ID_PATH_CORNER,
 MODEL_ID_POWERUP_RAPID,
 MODEL_ID_POWERUP_SPREAD,

 NUM_MODELS,
 MODEL_ID_NO_MODEL = NUM_MODELS
};








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

