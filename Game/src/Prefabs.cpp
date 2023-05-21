#include "Prefabs.h"
#include "EntityComponentSystem.h"
using namespace EntityComponentSystem;


std::list<Entity> createProjectileBasic() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::tags[e] = 0;
    GameData::states[e] = 0;
    GameData::activity[e] = true;
    GameData::positions[e] = glm::vec3(0, 0, -4);
    GameData::velocities[e].velocity = glm::vec3(0, 0, -0.5);
    GameData::colliders[e] = { glm::vec3(1, 1, 1) };
    GameData::models[e].modelID = MODEL_ID_PROJECTILE;
    GameData::models[e].asciiRep = 'J';
    GameData::coldmg[e].damage = 30.0f;
    GameData::lifespans[e] = 5;
    GameData::spawnrates[e] = 0.25;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj;


    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::DiesOnCollision +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::LifeSpan;
    return createdEntities;
};
std::list<Entity> createProjectileSpread5() {
    std::list<Entity> createdEntities;
    for (int i = 0; i < 5; ++i) {
        Entity e = createEntity();
        createdEntities.push_back(e);
        if (e == INVALID_ENTITY) {
            return createdEntities;
        }
        GameData::activity[e] = true;
        GameData::positions[e] = glm::vec3(2 - i, 0, -4);
        GameData::velocities[e].velocity = glm::vec3((2 - i), 0, -0.5);
        GameData::colliders[e] = { glm::vec3(.25, .25, .25) };
        GameData::models[e].modelID = MODEL_ID_PROJECTILE;
        GameData::models[e].asciiRep = 'J';
        GameData::coldmg[e].damage = 30.0f;
        GameData::lifespans[e] = 1;
        GameData::spawnrates[e] = 0.25;
        GameData::colliders[e].colteam = CollisionLayer::WorldObj;
        GameData::colliders[e].colwith = CollisionLayer::WorldObj;


        GameData::tags[e] =
            ComponentTags::Position +
            ComponentTags::Velocity +
            ComponentTags::Model +
            ComponentTags::Collidable +
            ComponentTags::DiesOnCollision +
            ComponentTags::CollisionDmg +
            ComponentTags::Hostility +
            ComponentTags::LifeSpan;
    }
    return createdEntities;
};
std::list<Entity> createProjectileChaos() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::activity[e] = true;
    GameData::positions[e] = glm::vec3(0, 0, -4);
    GameData::velocities[e].velocity = glm::vec3(0, 0, -0.1);
    GameData::colliders[e] = { glm::vec3(.25, .25, .25) };
    GameData::models[e].modelID = MODEL_ID_PROJECTILE;
    GameData::models[e].asciiRep = 'J';
    GameData::coldmg[e].damage = 30.0f;
    GameData::lifespans[e] = 10;
    GameData::spawnrates[e] = 1;
    GameData::pattackmodules[e].cooldown = 0.1;
    GameData::pattackmodules[e].attack = Prefabs::ProjectileRandom;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj;


    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::DiesOnCollision +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::LifeSpan + 
        ComponentTags::Attacker;
    return createdEntities;
}
std::list<Entity> createProjectileRandom() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::activity[e] = true;
    glm::vec3 randvec = glm::normalize(glm::vec3(rand() % 64 - 32, rand() % 64 - 32, rand() % 64 - 32));
    GameData::positions[e] = randvec * 4.0f;
    GameData::velocities[e].velocity = randvec * 0.5f;
    GameData::colliders[e] = { glm::vec3(.25, .25, .25) };
    GameData::models[e].modelID = MODEL_ID_PROJECTILE;
    GameData::models[e].asciiRep = 'J';
    GameData::coldmg[e].damage = 30.0f;
    GameData::lifespans[e] = 0.5;
    GameData::spawnrates[e] = 0.1;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj;


    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::DiesOnCollision +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::LifeSpan;
    return createdEntities;
}
std::list<Entity> createEnemyGroundBasic() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::activity[e] = true;
    GameData::states[e] = enemyState::Pathing;
    GameData::pathStructs[e].currentNode = 0;
    GameData::pathStructs[e].path = 0;
    GameData::velocities[e].moveSpeed = ENEMY_GND_BASE_MVSPD;
    GameData::velocities[e].flying = false;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e] = { glm::vec3(1, 1, 1) };
    GameData::models[e].modelID = MODEL_ID_MOB;
    GameData::models[e].asciiRep = 'E';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH;
    GameData::coldmg[e].damage = ENEMY_GND_BASE_DMG;
    GameData::hostilities[e].team = Teams::Martians;
    GameData::hostilities[e].hostileTo = Teams::Players + Teams::Towers;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj;
    GameData::pointvalues[e] = 20;
    GameData::resources[e].resources[ResourceType::Money] = 20;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::PathData +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::WorthPoints +
        ComponentTags::ResourceContainer;

    return createdEntities;
};
std::list<Entity> createEnemyGroundTank() {
    std::list<Entity> createdEntities;
    Entity e = createEntity(ENEMY_START, ENEMY_END);
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::activity[e] = true;
    GameData::states[e] = enemyState::Pathing;
    GameData::pathStructs[e].currentNode = 0;
    GameData::pathStructs[e].path = 0;
    GameData::velocities[e].flying = false;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e] = { glm::vec3(1, 1, 1) };
    GameData::models[e].asciiRep = 'E';
    GameData::hostilities[e].team = Teams::Martians;
    GameData::hostilities[e].hostileTo = Teams::Players + Teams::Towers;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj;
    GameData::pointvalues[e] = 20;
    GameData::resources[e].resources[ResourceType::Money] = 20;
    //distinguishing factors
    GameData::velocities[e].moveSpeed = ENEMY_GND_BASE_MVSPD / 2;
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH * 4;
    GameData::coldmg[e].damage = ENEMY_GND_BASE_DMG * 2;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::PathData +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::WorthPoints +
        ComponentTags::ResourceContainer;

    return createdEntities;
};
std::list<Entity> createEnemyFlyingBasic() {
    std::list<Entity> createdEntities;
    Entity e = createEntity(ENEMY_START, ENEMY_END);
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::activity[e] = true;
    GameData::states[e] = enemyState::Pathing;
    GameData::pathStructs[e].currentNode = 0;
    GameData::pathStructs[e].path = 0;
    GameData::velocities[e].moveSpeed = ENEMY_GND_BASE_MVSPD;
    GameData::velocities[e].flying = true;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e] = { glm::vec3(1, 1, 1) };
    GameData::models[e].asciiRep = 'E';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH;
    GameData::coldmg[e].damage = ENEMY_GND_BASE_DMG;
    GameData::hostilities[e].team = Teams::Martians;
    GameData::hostilities[e].hostileTo = Teams::Players + Teams::Towers;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj;
    GameData::pointvalues[e] = 20;
    GameData::resources[e].resources[ResourceType::Money] = 20;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::PathData +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::WorthPoints +
        ComponentTags::ResourceContainer;

    return createdEntities;
};

std::list<Entity> createTowerReticle() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::activity[e] = true;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::models[e].modelID = MODEL_ID_TOWER;
    GameData::models[e].asciiRep = 'T';
    GameData::hostilities[e].team = Teams::Towers;
    GameData::hostilities[e].hostileTo = Teams::Martians;
    GameData::colliders[e] = { glm::vec3(1, 1, 1) };
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::Collidable +
        ComponentTags::DiesOnCollision;
    GameData::colliders[e].colteam = CollisionLayer::UIObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::UIObj;

    return createdEntities;
}

std::list<Entity> createTowerBasic() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::activity[e] = true;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::turrets[e].damage = TURRET_BASE_DMG;
    GameData::turrets[e].range = 5;
    GameData::models[e].modelID = MODEL_ID_TOWER;
    GameData::models[e].asciiRep = 'T';
    GameData::hostilities[e].team = Teams::Towers;
    GameData::hostilities[e].hostileTo = Teams::Martians;
    GameData::colliders[e] = { glm::vec3(1, 1, 1) };
    GameData::rigidbodies[e].fixed = true;
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::RigidBody +
        ComponentTags::Collidable;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj;

    return createdEntities;
};


std::list<Entity> createHome() {
    std::list<Entity> createdEntities;
    Entity e = createEntity(BASE_START, BASE_END);
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::activity[e] = true;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::models[e].asciiRep = 'B';
    GameData::hostilities[e].team = Teams::Towers;
    GameData::hostilities[e].hostileTo = Teams::Martians;
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = HOME_BASE_HEALTH;
    GameData::colliders[e] = { glm::vec3(1, 1, 1) };
    GameData::rigidbodies[e].fixed = true;
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Turret +
        ComponentTags::Hostility +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::Collidable;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj;

    return createdEntities;
}

std::list<Entity> createPlayers() {
    std::list<Entity> createdEntities;

    //Initialize Players
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        Entity e = createEntity(0, NUM_PLAYERS);
        createdEntities.push_back(e);
        if (e == INVALID_ENTITY) {
            return createdEntities;
        }
        GameData::activity[e] = true;
        GameData::positions[e] = glm::vec3(0, 0, 0);
        GameData::velocities[e].velocity = glm::vec3(0, 0, 0);
        GameData::colliders[e] = { glm::vec3(1, 1, 1) };
        GameData::models[e].modelID = MODEL_ID_ROVER;
        GameData::models[e].asciiRep = 'P';
        //GameData::models[i].renderCollider = true;
        GameData::healths[e].maxHealth = GameData::healths[i].curHealth = PLAYER_BASE_HEALTH;
        GameData::hostilities[e].team = Teams::Players;
        GameData::hostilities[e].hostileTo = Teams::Environment + Teams::Martians;
        GameData::pattackmodules[e].attack = Prefabs::ProjectileBasic;
        GameData::pattackmodules[e].targetPos = glm::vec3(0, 0, 0);
        GameData::pattackmodules[e].cooldown = 0;
        GameData::states[e] = 0;
        GameData::retplaces[e].buildingPrefab = Prefabs::TowerBasic;
        GameData::retplaces[e].reticlePrefab = Prefabs::TowerReticle;
        GameData::retplaces[e].reticle = INVALID_ENTITY;
        GameData::retplaces[e].place = false;
        GameData::retplaces[e].validTarget = false;
        GameData::colliders[e].colteam = CollisionLayer::WorldObj;
        GameData::colliders[e].colwith = CollisionLayer::WorldObj;

        GameData::tags[e] =
            ComponentTags::Position +
            ComponentTags::Velocity +
            ComponentTags::Model +
            ComponentTags::Collidable +
            ComponentTags::RigidBody +
            ComponentTags::Health +
            ComponentTags::Hostility;
        //TODO: Other Model Data
    }
    //TODO: Change
    //Manually set spawn positions
    GameData::positions[0] = glm::vec3(0, 0, 0);
    GameData::positions[1] = glm::vec3(0, 0, 3);
    GameData::positions[2] = glm::vec3(3, 0, 0);
    GameData::positions[3] = glm::vec3(3, 0, 3);
    return createdEntities;
}

std::list<Entity> createWoodResourceBasic()
{
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::activity[e] = true;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e] = { glm::vec3(1, 1, 1) };
    GameData::models[e].asciiRep = 'R';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = RESOURCE_BASE_HEALTH;
    GameData::hostilities[e].team = Teams::Environment;
    GameData::models[e].modelID = MODEL_ID_RESOURCE;
    GameData::hostilities[e].hostileTo = 0;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj;
    GameData::resources[e].resources[ResourceType::Wood] = 20;
    GameData::rigidbodies[e].fixed = true;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::Hostility +
        ComponentTags::ResourceContainer;

    return createdEntities;
}
std::list<Entity> createStoneResourceBasic()
{
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::activity[e] = true;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e] = { glm::vec3(1, 1, 1) };
    GameData::models[e].asciiRep = 'R';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = RESOURCE_BASE_HEALTH;
    GameData::hostilities[e].team = Teams::Environment;
    GameData::hostilities[e].hostileTo = 0;
    GameData::models[e].modelID = MODEL_ID_RESOURCE;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj;
    GameData::resources[e].resources[ResourceType::Stone] = 20;
    GameData::rigidbodies[e].fixed = true;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::Hostility +
        ComponentTags::ResourceContainer;

    return createdEntities;
}


namespace Paths {
    const glm::vec3 path[pathCount][PATH_LENGTH] =
    {
        { glm::vec3(60,0,0), glm::vec3(40,0,0), glm::vec3(40,0,20), glm::vec3(20,0,20), glm::vec3(20,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) },
        { glm::vec3(-60,0,0), glm::vec3(-40,0,0), glm::vec3(-40,0,-20), glm::vec3(-20,0,-20), glm::vec3(-20,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) },
        { glm::vec3(0,0,60), glm::vec3(0,0,40), glm::vec3(-20,0,40), glm::vec3(-20,0,20), glm::vec3(0,0,20), glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) },
        { glm::vec3(0,0,-60), glm::vec3(0,0,-40), glm::vec3(20,0,-40), glm::vec3(20,0,-20), glm::vec3(0,0,-20), glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) }
    };
}
namespace WaveData {
    int currentWave;

    // How much time a wave lasts before the next wave starts spawning (timer for last wave will be time until win)
    int waveTimers[WAVE_COUNT] = { 5 * TICK_RATE, 5 * TICK_RATE, 5 * TICK_RATE, 5 * TICK_RATE, 5 * TICK_RATE };

    int waveTick; //countdown timer for waves

    int enemyTypes[NUM_ENEMY_TYPES] = { Prefabs::EnemyGroundBasic, Prefabs::EnemyGroundTank, Prefabs::EnemyFlyingBasic };

    std::queue<enemy> waves[WAVE_COUNT];
}

//Define Tower Build costs
const std::array<std::array<int, NUM_RESOURCE_TYPES>, NUM_TOWER_PREFAB> buildcosts =
{
    {20, 20, 20}
};

std::list<Entity> createPathColliders()
{
    std::list<Entity> createdEntities;
    for (int p = 0; p < Paths::pathCount; ++p) {
        for (int i = 0; (i < PATH_LENGTH - 1); i++) {
            if (Paths::path[p][i] == Paths::path[p][i + 1]) {
                continue;
            }
            Entity e = createEntity();
            createdEntities.push_back(e);
            if (e == INVALID_ENTITY) {
                return createdEntities;
            }

            GameData::activity[e] = true;
            GameData::positions[e] = (Paths::path[p][i] + Paths::path[p][i + 1]) / 2.0f + glm::vec3(0, 1, 0);
            glm::vec3 pathvec = Paths::path[p][i + 1] - Paths::path[p][i];
            GameData::colliders[e] = { (pathvec) / 2.0f + PATH_WIDTH * glm::normalize(pathvec) + PATH_WIDTH * glm::normalize(glm::vec3(pathvec.z, 0, -pathvec.x)) + glm::vec3(0, 1,0) };
            
            GameData::colliders[e].AABB.x = glm::abs(GameData::colliders[e].AABB.x);
            GameData::colliders[e].AABB.y = glm::abs(GameData::colliders[e].AABB.y);
            GameData::colliders[e].AABB.z = glm::abs(GameData::colliders[e].AABB.z);

            GameData::models[e].modelID = MODEL_ID_NO_MODEL;
            GameData::models[e].asciiRep = 'P';
            GameData::models[e].renderCollider = true;
            GameData::colliders[e].colteam = CollisionLayer::UIObj;
            GameData::colliders[e].colwith = 0;

            GameData::tags[e] =
                ComponentTags::Position +
                ComponentTags::Model +
                ComponentTags::Collidable;
        }
    }

    return createdEntities;
};
