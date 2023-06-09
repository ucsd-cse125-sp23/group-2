#pragma once
#include <vector>
#include <queue>
#include "PoissonDisk.h"
#include <chrono>
#include <unordered_set>
#include <array>
#include "graphics/core.h"
#include "GameConstants.h"
#include "Prefabs.h"
#include <math.h>

//Entity type
using Entity = uint32_t;
const Entity INVALID_ENTITY = MAX_ENTITIES;

//Define Component Tpyes (Max Components = 32)


using LifeSpan = float;

using Creator = Entity;

using SpawnRate = float;

//Define Component Tags
using Tag = uint32_t;


using Active = bool; //Is Entity active in the scene?
using Position = glm::vec3; //Entity Position in 3D Space
struct VelocityData
{
    glm::vec3 velocity; //vector that is added to current position each tick
    float moveSpeed; //scalar multiplied by velocity vector to modify speed
    bool flying; //modify the height of the target positions to hover above them
};
using TeamID = uint32_t;
using State = Tag;
using Points = int;

namespace Teams {
    constexpr TeamID Players = 0x1;
    constexpr TeamID Martians = 0x1 << 1;
    constexpr TeamID Towers = 0x1 << 2;
    constexpr TeamID Environment = 0x1 << 3;
    constexpr TeamID Projectile = 0x1 << 4;
    constexpr TeamID Powerup = 0x1 << 5;
    constexpr TeamID Home = 0x1 << 6;
}

namespace ResourceType {
    const int Money = 0;
    const int Stone = 1;
    const int Wood = 2;
};

namespace CollisionLayer {
    constexpr TeamID WorldObj = 0x1;
    constexpr TeamID UIObj = 0x1 << 1;
    constexpr TeamID StaticObj = 0x1 << 2;
    constexpr TeamID Players = 0x1 << 3;
    constexpr TeamID Boss = 0x1 << 4;
    constexpr TeamID Powerup = 0x1 << 5;
}

struct Hostility {
    TeamID team;
    TeamID hostileTo;
};

struct PathData //Data for entity pathing
{
    int path; //The chosen path of the entity ( Paths::path[path#][node#] )
    int currentNode; //Index of current node that entity is pathing towards
};

struct HomingData //Data for tracking another entity
{
    Entity trackedEntity; //The entity to follow (player, tower, enemy, etc...)
    glm::vec3 offset; //A 3d offset to track
};

struct Model //3D Model to render for the entity
{
    MODEL_ID modelID;
    char asciiRep;
    glm::vec3 dirNorm;
    bool renderCollider;
    bool upgradeSelected;


    //degrees
    float modelOrientation;

    float scale = 1;
};

struct Turret //Component of Towers
{
    float range; //The range of the tower
    State attackState;
};

namespace Collision {
    //Side should be bigger than the greatest sidelength of any AABB in the game
    constexpr float side = 8.0;
    static constexpr size_t gridx = (WORLD_X / side) + 1l;
    static constexpr size_t gridz = (WORLD_Z / side) + 1l;
    extern std::unordered_set<Entity> cgrid[gridx][gridz];
    void updateColTable(Entity e);
}

struct Collider //Information for collisions
{
    glm::vec3 AABB; //Axis Aligned Bound Box vector
    TeamID colteam;
    TeamID colwith;
    int xpos;
    int zpos;
    bool collided; //If has collided with anything this tick
};

struct RigidBodyInfo //Information for physical objects;
{
    bool fixed;
    bool grounded; //If object can be treated as being on groun (used for player jumping for now)
};

struct AbductionData
{
    float abductionTimeLeft;
    Entity captive;
};

struct CollisionEvent {
    Entity e;
    Entity o;
    glm::vec3 pen;
};

struct Health {
    float maxHealth;
    float curHealth;
};

struct CollisionDmg {
    float damage;
    float cooldown;
    float damageRate;
};

struct ProjectileAttackModule {
    Prefab attack;
    float cooldown; //Remaining coooldown in seconds
    glm::vec3 targetPos;
};

struct HitscanAttackModule {
    Entity target;
    float damage; //The damage that the dealt per second
    float cooldown;
    float fireRate;
};

struct AOEAttackModule {
    glm::vec3 source;
    float range;
    float damage; //The damage that the dealt per aoe blast
    float cooldown; //Cooldown between AOEs
    float fireRate;
};

struct ReticlePlacement {
    bool place;
    Prefab reticlePrefab;
    Prefab buildingPrefab;
    Entity reticle = INVALID_ENTITY;
    bool validTarget;
    glm::vec3 targetPos;
    float targetOrientation;
    bool renderRet;
};

struct CombatLog {
    Entity source;
    Entity target;
    float damage;
    bool killed;
};

struct SoundLog {
    Entity source;
    int sound;
    bool stop = false;
};

struct ScoreCard {
    int towersBuilt;
    int enemiesKilled;
    std::array<int, NUM_RESOURCE_TYPES> resourcesGathered;
    Points points;
};

struct AllPlayerData {
    std::array<ScoreCard, NUM_PLAYERS> scores;
    std::array<int, NUM_RESOURCE_TYPES> resources;
    std::array<float, NUM_PLAYERS> spawntimers;
    std::array<int, NUM_PLAYERS> actioncooldown;
    std::array<State, NUM_PLAYERS> playerStates;
    std::array<float, NUM_PLAYERS> powerupTimers;
    std::array<Entity, NUM_PLAYERS> selectedTowerUpgrade;
    std::array<std::array<int, NUM_RESOURCE_TYPES>, NUM_PLAYERS> selectedTowerUpgradeCost;
};

struct ResourceContainer {
    std::array<int, NUM_RESOURCE_TYPES> resources;
};

struct Upgradeable {
    std::array<int, NUM_RESOURCE_TYPES> cost;
    Prefab upgrade;
};

struct Powerup {
    Prefab newAttack;
};

namespace ComponentTags
{
    constexpr Tag Position  = 0x1 << 0;
    constexpr Tag Velocity  = 0x1 << 1;
    constexpr Tag PathData  = 0x1 << 2;
    constexpr Tag Model = 0x1 << 3;
    constexpr Tag Collidable = 0x1 << 4;
    constexpr Tag DiesOnCollision = 0x1 << 5;
    constexpr Tag RigidBody = 0x1 << 6;
    constexpr Tag Health = 0x1 << 7;
    constexpr Tag CollisionDmg = 0x1 << 8;
    constexpr Tag Turret = 0x1 << 9;
    constexpr Tag Hostility = 0x1 << 10;
    constexpr Tag AttackerProjectile = 0x1 << 11;
    constexpr Tag AttackerHitscan = 0x1 << 12;
    constexpr Tag AttackerAOE = 0x1 << 13;
    constexpr Tag LifeSpan = 0x1 << 14;
    constexpr Tag Created = 0x1 << 15;
    constexpr Tag Builder = 0x1 << 16;
    constexpr Tag Abductor = 0x1 << 17;
    constexpr Tag Abducted = 0x1 << 18;
    constexpr Tag HomingData = 0x1 << 19;
    constexpr Tag Dead = 0x1 << 20;
    constexpr Tag ResourceContainer = 0x1 << 21;
    constexpr Tag WorthPoints = 0x1 << 22;
    constexpr Tag Stalker = 0x1 << 23;
    constexpr Tag Hunter = 0x1 << 24;
    constexpr Tag Trapper = 0x1 << 25;
    constexpr Tag BarrierReticle = 0x1 << 26;
    constexpr Tag Upgradeable = 0x1 << 27;
    constexpr Tag Upgrading = 0x1 << 28;
    constexpr Tag PAttackInRangeAI = 0x1 << 29;
    constexpr Tag Powerup = 0x1 << 30;
}

namespace enemyState {
    constexpr State Pathing = ComponentTags::PathData;
    constexpr State Homing = ComponentTags::HomingData;
    constexpr State ShootingProjectile = ComponentTags::AttackerProjectile | ComponentTags::HomingData;
};

namespace towerStates {
    constexpr State Idle = 0;
    constexpr State AttackingHitscan = ComponentTags::AttackerHitscan;
    constexpr State AttackingProjectile = ComponentTags::AttackerProjectile;
};

struct PlayerState {
    static constexpr State Default = 0;
    static constexpr State Attack = ComponentTags::AttackerProjectile;
    static constexpr State Build = ComponentTags::Builder;
    static constexpr State Upgrading = ComponentTags::Upgrading;
};

namespace GameData
{
    //List of entities index by entity ID
    //Entity Tag is a 32 bit int that denotes the components attached to the enitity
    extern std::array<Tag, MAX_ENTITIES> tags;

    extern std::array<Active, MAX_ENTITIES> activity;
    extern std::array<Position, MAX_ENTITIES> positions;
    extern std::array<VelocityData, MAX_ENTITIES> velocities;
    extern std::array<PathData, MAX_ENTITIES> pathStructs;
    extern std::array<Model, MAX_ENTITIES> models;
    extern std::array<Turret, MAX_ENTITIES> turrets;
    extern std::array<Collider, MAX_ENTITIES> colliders;
    extern std::array<RigidBodyInfo, MAX_ENTITIES> rigidbodies;
    extern std::array<Health, MAX_ENTITIES> healths;
    extern std::array<CollisionDmg, MAX_ENTITIES> coldmg;
    extern std::array<Hostility, MAX_ENTITIES> hostilities;
    extern std::array<ProjectileAttackModule, MAX_ENTITIES> pattackmodules;
    extern std::array<HitscanAttackModule, MAX_ENTITIES> hattackmodules;
    extern std::array<AOEAttackModule, MAX_ENTITIES> AOEattackmodules;
    extern std::array<LifeSpan, MAX_ENTITIES> lifespans;
    extern std::array<Creator, MAX_ENTITIES> creators;
    extern std::array<SpawnRate, MAX_ENTITIES> spawnrates;
    extern std::array<State, MAX_ENTITIES> states;
    extern std::array<ReticlePlacement, MAX_ENTITIES> retplaces;
    extern std::array<HomingData, MAX_ENTITIES> homingStructs;
    extern std::array<AbductionData, MAX_ENTITIES> abductionStructs;
    extern std::array<ResourceContainer, MAX_ENTITIES> resources;
    extern std::array<Points, MAX_ENTITIES> pointvalues;
    extern std::array<Upgradeable, MAX_ENTITIES> upgradedata;
    extern std::array<Powerup, MAX_ENTITIES> powerupdata;
    //Events
    extern std::queue<CollisionEvent> colevents;

    //Logs for Client
    extern int clogpos;
    extern std::array<CombatLog, CLOG_MAXSIZE> combatLogs;
    extern int slogpos;
    extern std::array<SoundLog, SLOG_MAXSIZE> soundLogs;
    extern AllPlayerData playerdata;
}

namespace EntityComponentSystem
{
    //Systems
    //Contains Calls to ALL systems
    void update();

    //Move entities that have a velocity component
    void sysMovement();

    //Apply gravity to all rigid bodies
    void sysGravity();

    //Do pathfinding for entities that have a path component
    void sysPathing();

    //Detect Collisions
    void sysDetectCollisions();

    //Handle&Resolve Collisions
    void resolveCollisions();

    //All automated turret / tower firing
    void sysTurret();

    //Check the status of entity's HP
    void sysDeathStatus();

    //Attacks!
    void sysAttacks();

    //LifeSpan
    void sysLifeSpan();

    //Building shit
    void sysBuild();

    //tracking entities
    void sysHoming();

    //abducting entities
    void sysAbduction();

    void sysEnemyAI();

    void sysPowerup();

    //Helper functions
    Entity createEntity(int begin = 0, int end = MAX_ENTITIES);

    //Find the position of inte rsection with first rigid body (uses Peter Shirley's method at http://psgraphics.blogspot.com/2016/02/new-simple-ray-box-test-from-andrew.html)
    glm::vec3 computeRaycast(glm::vec3& pos, glm::vec3& dir, float tmin, float tmax, Entity * out = 0);

    //Deals damage (And will eventuall call death functions)
    void dealDamage(Entity source, Entity target, float damage);

    void causeDeath(Entity source, Entity target);

    //Creates collisions between two objects if they collide
    bool colCheck(Entity e, Entity o);

    //Finds the closest path node to an enemy to put them back on track
    void rePath(Entity e);

    void changeState(Entity e, State post);

    void logSound(Entity source, int sound_id, bool stop = false);

    //Get all Entitis is range
    std::list<Entity> getTargetsInRange(glm::vec3 & source, float & range, TeamID & hostileTo);

    //Finds closes path collider
    Entity findClosestPathCollider(glm::vec3 origin);

    //Applies an Upgrade if possible
    bool applyUpgrade(Entity play, Entity target);
};
