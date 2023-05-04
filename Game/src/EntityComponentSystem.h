#pragma once
#include <vector>
#include <queue>

#include <array>
#include "graphics/core.h"
#include "GameConstants.h"
#include <math.h>

//Entity type
using Entity = uint32_t;
const Entity INVALID_ENTITY = MAX_ENTITIES;

//Define Component Tpyes (Max Components = 32)
using Active = bool; //Is Entity active in the scene?
using Position = glm::vec3; //Entity Position in 3D Space
using Velocity = glm::vec3; //Entity Velocity in 3D Space

using AttackID = uint32_t;

using TeamID = uint32_t;
namespace Teams {
    constexpr TeamID Players = 0x1;
    constexpr TeamID Martians = 0x1 << 1;
    constexpr TeamID Towers = 0x1 << 2;
    constexpr TeamID Environment = 0x1 << 3;
}

struct Hostility {
    TeamID team;
    TeamID hostileTo;
};

struct PathData //Data for entity pathing
{
    glm::vec3 pathNodes[PATH_LENGTH]; //3D positions in which entity will need to change direction
    int currentNode; //Index of current node that entity is pathing towards
    float moveSpeed; //distance enemy covers in 1 server tick
};

struct Model //3D Model to render for the entity
{
    int modelID;
    char asciiRep;
    glm::vec3 dirNorm;
    //TODO: Other Model Data

    //degrees
    float modelOrientation;
};

struct Turret //Component of Towers
{
    float range; //The range of the tower
    float damage; //The damage that the turret deals per second
};

struct Collider //Information for collisions
{
    glm::vec3 AABB; //Axis Aligned Bound Box vector

    //TODO: Pointer to a mesh for narrow phase
};

struct RigidBodyInfo //Information for collisions
{
    bool fixed;
    int mass;
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
};

struct AttackModule {
    bool isAttacking;
    AttackID attack;
    int cooldown; //Remaining coooldown in ticks
    glm::vec3 targetPos;
};

using LifeSpan = float;

//Define Component Tags
using Tag = uint32_t;
namespace ComponentTags
{
    constexpr Tag Active    = 0x1;
    constexpr Tag Position  = 0x1 << 1;
    constexpr Tag Velocity  = 0x1 << 2;
    constexpr Tag PathData  = 0x1 << 3;
    constexpr Tag Model = 0x1 << 4;
    constexpr Tag Collidable = 0x1 << 5;
    constexpr Tag DiesOnCollision = 0x1 << 6;
    constexpr Tag RigidBody = 0x1 << 7;
    constexpr Tag Health = 0x1 << 8;
    constexpr Tag CollisionDmg = 0x1 << 9;
    constexpr Tag Turret = 0x1 << 10;
    constexpr Tag Hostility = 0x1 << 11;
    constexpr Tag Attacker = 0x1 << 12;
    constexpr Tag LifeSpan = 0x1 << 13;

}



namespace GameData
{
    //List of entities index by entity ID
    //Entity Tag is a 32 bit int that denotes the components attached to the enitity
    extern std::array<Tag, MAX_ENTITIES> tags;

    extern std::array<Active, MAX_ENTITIES> activity;   
    extern std::array<Position, MAX_ENTITIES> positions;
    extern std::array<Velocity, MAX_ENTITIES> velocities;
    extern std::array<PathData, MAX_ENTITIES> pathStructs;
    extern std::array<Model, MAX_ENTITIES> models;
    extern std::array<Turret, MAX_ENTITIES> turrets;
    extern std::array<Collider, MAX_ENTITIES> colliders;
    extern std::array<RigidBodyInfo, MAX_ENTITIES> rigidbodies;
    extern std::array<Health, MAX_ENTITIES> healths;
    extern std::array<CollisionDmg, MAX_ENTITIES> coldmg;
    extern std::array<Hostility, MAX_ENTITIES> hostilities;
    extern std::array<AttackModule, MAX_ENTITIES> attackmodules;
    extern std::array<LifeSpan, MAX_ENTITIES> lifespans;

    //Events
    extern std::queue<CollisionEvent> colevents;
}

namespace EntityComponentSystem
{
    //Systems
    //Contains Calls to ALL systems
    void update();

    //Move entities that have a velocity component
    void sysMovement();

    //Do pathfinding for entities that have a path component
    void sysPathing();

    //Detect Collisions
    void sysDetectCollisions();

    //Handle&Resolve Collisions
    void resolveCollisions();


    //All automated turret / tower firing
    void sysTurretFire();

    //Check the status of entity's HP
    void sysHealthStatus();

    //Attacks!
    void sysAttacks();

    //LifeSpan
    void sysLifeSpan();

    //Helper functions
    Entity createEntity();

    //Temp function (to be repalced withload from prefab)
    Entity createProjectile();
    
    //TODO: Implement Prefabs
    Entity loadPrefab();

    //Find the position of intersection with first rigid body (uses Peter Shirley's method at http://psgraphics.blogspot.com/2016/02/new-simple-ray-box-test-from-andrew.html)
    glm::vec3 computeRaycast(glm::vec3& pos, glm::vec3& dir, float tmin, float tmax);
};
