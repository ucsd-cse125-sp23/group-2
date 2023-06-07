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
    GameData::positions[e] = glm::vec3(0, 0, -2);
    GameData::velocities[e].velocity = glm::vec3(0, 0, -1)*PROJ_MVSPD;
    GameData::colliders[e].AABB =  glm::vec3(1, 1, 1);
    GameData::models[e].modelID = MODEL_ID_PROJECTILE;
    GameData::models[e].asciiRep = 'J';
    GameData::coldmg[e].damage = 40.0f;
    GameData::lifespans[e] = 5;
    GameData::spawnrates[e] = PROJ_SPAWN_RATE;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj + CollisionLayer::Boss;
    GameData::hostilities[e].team = Teams::Projectile;
    GameData::coldmg[e].cooldown = 0;
    GameData::coldmg[e].damageRate = PROJ_DAMAGE_RATE;

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
        GameData::positions[e] = glm::vec3(2 - i, 0, -2);
        GameData::velocities[e].velocity = glm::normalize(glm::vec3((float)(2 - i) * 0.1, 0, -0.5)) * (PROJ_MVSPD/2);
        GameData::colliders[e].AABB =  glm::vec3(.25, .25, .25);
        GameData::models[e].modelID = MODEL_ID_PROJECTILE;
        GameData::models[e].asciiRep = 'J';
        GameData::coldmg[e].damage = 20.0f;
        GameData::lifespans[e] = 1;
        GameData::spawnrates[e] = PROJ_SPAWN_RATE*1.5;
        GameData::colliders[e].colteam = CollisionLayer::WorldObj;
        GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj + CollisionLayer::Boss;
        GameData::hostilities[e].team = Teams::Projectile;
        GameData::coldmg[e].cooldown = 0;
        GameData::coldmg[e].damageRate = PROJ_DAMAGE_RATE;

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
std::list<Entity> createProjectilePierce() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::positions[e] = glm::vec3(0, 0, -1);
    GameData::velocities[e].velocity = glm::vec3(0, 0, -5) * PROJ_MVSPD;
    GameData::colliders[e].AABB = glm::vec3(1, 1, 1);
    GameData::models[e].modelID = MODEL_ID_PROJECTILE;
    GameData::models[e].asciiRep = 'J';
    GameData::coldmg[e].damage = 20.0f;
    GameData::lifespans[e] = 5;
    GameData::spawnrates[e] = PROJ_SPAWN_RATE * 4;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj + CollisionLayer::Boss;
    GameData::hostilities[e].team = Teams::Projectile;
    GameData::coldmg[e].cooldown = 0;
    GameData::coldmg[e].damageRate = 0;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::LifeSpan;
    return createdEntities;
};

std::list<Entity> createProjectilePierce1() {
    std::list<Entity> createdEntities;
    Entity e = createProjectilePierce().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    //distinguishing factors
    GameData::models[e].modelID = MODEL_ID_PROJECTILE;
    GameData::spawnrates[e] /= 1.2;


    return createdEntities;
};

std::list<Entity> createProjectilePierce2() {
    std::list<Entity> createdEntities;
    Entity e = createProjectilePierce().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    //distinguishing factors
    GameData::models[e].modelID = MODEL_ID_PROJECTILE;
    GameData::spawnrates[e] /= 1.5;


    return createdEntities;
};

std::list<Entity> createProjectilePierce3() {
    std::list<Entity> createdEntities;
    Entity e = createProjectilePierce().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    //distinguishing factors
    GameData::models[e].modelID = MODEL_ID_PROJECTILE;
    GameData::spawnrates[e] /= 2;


    return createdEntities;
};

std::list<Entity> createProjectileSpray() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::positions[e] = glm::vec3(0, 0, -1);
    GameData::velocities[e].velocity = glm::normalize(glm::vec3(rand() % 64 - 32, rand() % 64 - 32, -256)) * PROJ_MVSPD;
    GameData::colliders[e].AABB = glm::vec3(.25, .25, .25);
    GameData::models[e].modelID = MODEL_ID_PROJECTILE;
    GameData::models[e].asciiRep = 'J';
    GameData::coldmg[e].damage = 6.0f;
    GameData::lifespans[e] = 0.25;
    GameData::spawnrates[e] = PROJ_SPAWN_RATE/10;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj + CollisionLayer::Boss;
    GameData::hostilities[e].team = Teams::Projectile;
    GameData::coldmg[e].cooldown = 0;
    GameData::coldmg[e].damageRate = PROJ_DAMAGE_RATE;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::LifeSpan;
    return createdEntities;
};
std::list<Entity> createProjectileChaos() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::positions[e] = glm::vec3(0, 0, -4);
    GameData::velocities[e].velocity = glm::vec3(0, 0, -0.1);
    GameData::colliders[e].AABB =  glm::vec3(.25, .25, .25);
    GameData::models[e].modelID = MODEL_ID_PROJECTILE;
    GameData::models[e].asciiRep = 'J';
    GameData::coldmg[e].damage = 30.0f;
    GameData::lifespans[e] = 10;
    GameData::spawnrates[e] = 1;
    GameData::pattackmodules[e].cooldown = PROJ_SPAWN_RATE/2;
    GameData::pattackmodules[e].attack = Prefabs::ProjectileRandom;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj + CollisionLayer::Boss;
    GameData::hostilities[e].team = Teams::Projectile;
    GameData::coldmg[e].cooldown = 0;
    GameData::coldmg[e].damageRate = PROJ_DAMAGE_RATE;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::DiesOnCollision +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::LifeSpan + 
        ComponentTags::AttackerProjectile;
    return createdEntities;
}
std::list<Entity> createProjectileRandom() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    glm::vec3 randvec = glm::normalize(glm::vec3(rand() % 64 - 32, rand() % 64 - 32, rand() % 64 - 32));
    GameData::positions[e] = randvec * 4.0f;
    GameData::velocities[e].velocity = randvec * 0.5f;
    GameData::colliders[e].AABB =  glm::vec3(.25, .25, .25);
    GameData::models[e].modelID = MODEL_ID_PROJECTILE;
    GameData::models[e].asciiRep = 'J';
    GameData::coldmg[e].damage = 30.0f;
    GameData::lifespans[e] = 0.5;
    GameData::spawnrates[e] = PROJ_SPAWN_RATE/2;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj + CollisionLayer::Boss;
    GameData::hostilities[e].team = Teams::Projectile;
    GameData::coldmg[e].cooldown = 0;
    GameData::coldmg[e].damageRate = PROJ_DAMAGE_RATE;

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
    GameData::states[e] = enemyState::Pathing;
    GameData::pathStructs[e].currentNode = 0;
    GameData::pathStructs[e].path = 0;
    GameData::velocities[e].moveSpeed = ENEMY_GND_BASE_MVSPD;
    GameData::velocities[e].flying = false;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e].AABB =  glm::vec3(1.5, 2, 1.5);
    GameData::models[e].modelID = MODEL_ID_MOB;
    GameData::models[e].asciiRep = 'E';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH;
    GameData::coldmg[e].damage = ENEMY_GND_BASE_DMG;
    GameData::hostilities[e].team = Teams::Martians;
    GameData::hostilities[e].hostileTo = Teams::Players + Teams::Towers;
    GameData::colliders[e].colteam = CollisionLayer::WorldObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj + CollisionLayer::Boss;
    GameData::resources[e].resources[ResourceType::Money] = 20;
    GameData::pointvalues[e] = 20;
    GameData::models[e].renderCollider = true;
    GameData::rigidbodies[e].fixed = false;
    GameData::rigidbodies[e].grounded = false;
    GameData::coldmg[e].cooldown = 0;
    GameData::coldmg[e].damageRate = ENEMY_BASE_DAMAGE_RATE;


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
        ComponentTags::ResourceContainer + 
        ComponentTags::Stalker;

    return createdEntities;
};

std::list<Entity> createEnemyGroundTank() {
    std::list<Entity> createdEntities;
    Entity e = createEnemyGroundBasic().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    //distinguishing factors
    GameData::models[e].modelID = MODEL_ID_MOB_TANK;
    GameData::colliders[e].AABB = glm::vec3(2.25, 3, 2.25);
    GameData::velocities[e].moveSpeed = ENEMY_GND_BASE_MVSPD / 2;
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH * 4;
    GameData::coldmg[e].damage = ENEMY_GND_BASE_DMG * 2;
    GameData::coldmg[e].damageRate = ENEMY_BASE_DAMAGE_RATE;

    return createdEntities;
};
std::list<Entity> createEnemyGroundMini() {
    std::list<Entity> createdEntities;
    Entity e = createEnemyGroundBasic().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    //distinguishing factors
    GameData::models[e].modelID = MODEL_ID_MOB_MINI;
    GameData::colliders[e].AABB = glm::vec3(1.125, 1.5, 1.125);
    GameData::velocities[e].moveSpeed = ENEMY_GND_BASE_MVSPD * 4;
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH/2;
    GameData::coldmg[e].damage = ENEMY_GND_BASE_DMG/2;
    GameData::coldmg[e].damageRate = ENEMY_BASE_DAMAGE_RATE;

    return createdEntities;
};
std::list<Entity> createEnemyFlyingBasic() {
    std::list<Entity> createdEntities;
    Entity e = createEnemyGroundBasic().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    //distinguishing factors
    GameData::pattackmodules[e].cooldown = 0;
    GameData::pattackmodules[e].attack = Prefabs::ProjectileBasic;
    GameData::velocities[e].flying = true;
    GameData::models[e].modelID = MODEL_ID_MOB_FLYING;
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
        ComponentTags::ResourceContainer +
        ComponentTags::Hunter;

    return createdEntities;
};

std::list<Entity> createEnemyFlyingTractor() {
    std::list<Entity> createdEntities;
    Entity e = createEnemyFlyingBasic().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    //distinguishing factors
    GameData::models[e].modelID = MODEL_ID_MOB_TRACTOR;
    GameData::abductionStructs[e].abductionTimeLeft = ABDUCT_TIMER;
    GameData::abductionStructs[e].captive = INVALID_ENTITY;
    GameData::homingStructs[e].trackedEntity = 0;
    GameData::states[e] = enemyState::Homing;
    GameData::coldmg[e].damage = 0.0f;
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
        ComponentTags::ResourceContainer +
        ComponentTags::Abductor +
        ComponentTags::HomingData +
        ComponentTags::Trapper;

    return createdEntities;
};

std::list<Entity> createTowerReticleBasic() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::models[e].modelID = MODEL_ID_TOWER;
    GameData::models[e].asciiRep = 'T';
    GameData::hostilities[e].team = Teams::Towers;
    GameData::hostilities[e].hostileTo = Teams::Martians;
    GameData::colliders[e].AABB =  glm::vec3(1.2, 1, 1.2);
    GameData::positions[e].y = GameData::colliders[e].AABB.y;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::Collidable +
        ComponentTags::DiesOnCollision;
    GameData::models[e].renderCollider = true;
    GameData::colliders[e].colteam = CollisionLayer::UIObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj + CollisionLayer::UIObj + CollisionLayer::Boss;

    return createdEntities;
}


std::list<Entity> createTowerReticleRailgun() {
    std::list<Entity> createdEntities;
    Entity e = createTowerReticleBasic().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    
    GameData::models[e].modelID = MODEL_ID_RAILGUN;
    
    GameData::colliders[e].AABB = glm::vec3(2, 2, 2);
    GameData::positions[e].y = GameData::colliders[e].AABB.y;
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::Collidable +
        ComponentTags::DiesOnCollision;
    GameData::models[e].renderCollider = true;
    
    return createdEntities;
}

std::list<Entity> createTowerReticleTesla() {
    std::list<Entity> createdEntities;
    Entity e = createTowerReticleBasic().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::models[e].modelID = MODEL_ID_TESLA;
    GameData::colliders[e].AABB = glm::vec3(1.8, 6.5, 1.8);
    GameData::positions[e].y = GameData::colliders[e].AABB.y;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::Collidable +
        ComponentTags::DiesOnCollision;
    GameData::models[e].renderCollider = true;

    return createdEntities;
}


std::list<Entity> createTowerReticleBarrier()
{
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::models[e].modelID = MODEL_ID_BARRIER;
    GameData::models[e].asciiRep = 'T';
    GameData::hostilities[e].team = Teams::Towers;
    GameData::hostilities[e].hostileTo = Teams::Martians;
    GameData::colliders[e].AABB = glm::vec3(2, 2, 5);
    GameData::positions[e].y = GameData::colliders[e].AABB.y;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::Collidable +
        ComponentTags::DiesOnCollision +
        ComponentTags::BarrierReticle;
    GameData::models[e].renderCollider = true;
    GameData::colliders[e].colteam = CollisionLayer::UIObj;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj + CollisionLayer::Boss;

    return createdEntities;
}


std::list<Entity> createTowerBasic() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::turrets[e].range = TURRET_BASE_RANGE;
    GameData::turrets[e].attackState = towerStates::AttackingHitscan;
    GameData::hattackmodules[e].damage = TURRET_BASE_DMG;
    GameData::hattackmodules[e].cooldown = 0;
    GameData::hattackmodules[e].fireRate = TURRET_BASE_FIRE_RATE;
    GameData::models[e].modelID = MODEL_ID_TOWER;
    GameData::models[e].asciiRep = 'T';
    GameData::hostilities[e].team = Teams::Towers;
    GameData::hostilities[e].hostileTo = Teams::Martians;
    GameData::colliders[e].AABB =  glm::vec3(1, 1, 1);
    GameData::rigidbodies[e].fixed = true;
    GameData::rigidbodies[e].grounded = false;
    GameData::upgradedata[e].cost = { 20, 20, 0 };
    GameData::upgradedata[e].upgrade = Prefabs::TowerBasic1;
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::RigidBody +
        ComponentTags::Collidable +
        ComponentTags::Turret +
        ComponentTags::Upgradeable;
    GameData::models[e].renderCollider = true;
    GameData::colliders[e].colteam = CollisionLayer::StaticObj;
    GameData::colliders[e].colwith = 0;

    return createdEntities;
};

std::list<Entity> createTowerBasic1() {
    std::list<Entity> createdEntities;
    Entity e = createTowerBasic().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }

    GameData::models[e].modelID = MODEL_ID_TOWER;
    GameData::upgradedata[e].cost = { 20, 20, 0 };
    GameData::upgradedata[e].upgrade = Prefabs::TowerBasic2;
    GameData::hattackmodules[e].damage *= 1.20;


    return createdEntities;
};

std::list<Entity> createTowerBasic2() {
    std::list<Entity> createdEntities;
    Entity e = createTowerBasic().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }

    GameData::models[e].modelID = MODEL_ID_TOWER;
    GameData::upgradedata[e].cost = { 100, 100, 0 };
    GameData::upgradedata[e].upgrade = Prefabs::TowerBasic3;
    GameData::hattackmodules[e].damage *= 2.00;


    return createdEntities;
};

std::list<Entity> createTowerBasic3() {
    std::list<Entity> createdEntities;
    Entity e = createTowerBasic().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }

    GameData::models[e].modelID = MODEL_ID_TOWER;
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::RigidBody +
        ComponentTags::Collidable +
        ComponentTags::Turret;
    GameData::hattackmodules[e].damage *= 5.00;


    return createdEntities;
};

std::list<Entity> createTowerRailgun() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::turrets[e].range = TURRET_BASE_RANGE*4;
    GameData::turrets[e].attackState = towerStates::AttackingProjectile;
    GameData::pattackmodules[e].cooldown = 0;
    GameData::pattackmodules[e].attack = Prefabs::ProjectilePierce;
    GameData::models[e].modelID = MODEL_ID_RAILGUN;
    GameData::models[e].asciiRep = 'T';
    GameData::hostilities[e].team = Teams::Towers;
    GameData::hostilities[e].hostileTo = Teams::Martians;
    GameData::colliders[e].AABB = glm::vec3(2, 2, 2);
    GameData::rigidbodies[e].fixed = true;
    GameData::rigidbodies[e].grounded = false;
    GameData::upgradedata[e].cost = { 20, 20, 0 };
    GameData::upgradedata[e].upgrade = Prefabs::TowerRailgun1;
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::RigidBody +
        ComponentTags::Collidable +
        ComponentTags::Turret +
        ComponentTags::Upgradeable;
    GameData::models[e].renderCollider = true;
    GameData::colliders[e].colteam = CollisionLayer::StaticObj;
    GameData::colliders[e].colwith = 0;

    return createdEntities;
};

std::list<Entity> createTowerRailgun1() {
    std::list<Entity> createdEntities;
    Entity e = createTowerRailgun().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }

    GameData::models[e].modelID = MODEL_ID_RAILGUN;
    GameData::upgradedata[e].cost = { 20, 20, 0 };
    GameData::upgradedata[e].upgrade = Prefabs::TowerRailgun2;
    GameData::pattackmodules[e].attack = Prefabs::ProjectilePierce1;


    return createdEntities;
};

std::list<Entity> createTowerRailgun2() {
    std::list<Entity> createdEntities;
    Entity e = createTowerRailgun().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }

    GameData::models[e].modelID = MODEL_ID_RAILGUN;
    GameData::upgradedata[e].cost = { 100, 100, 0 };
    GameData::upgradedata[e].upgrade = Prefabs::TowerRailgun3;
    GameData::pattackmodules[e].attack = Prefabs::ProjectilePierce2;


    return createdEntities;
};

std::list<Entity> createTowerRailgun3() {
    std::list<Entity> createdEntities;
    Entity e = createTowerRailgun().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }

    GameData::models[e].modelID = MODEL_ID_RAILGUN;
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::RigidBody +
        ComponentTags::Collidable +
        ComponentTags::Turret;
    GameData::pattackmodules[e].attack = Prefabs::ProjectilePierce3;


    return createdEntities;
};

std::list<Entity> createTowerTesla() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::models[e].modelID = MODEL_ID_TESLA;
    GameData::models[e].asciiRep = 'T';
    GameData::hostilities[e].team = Teams::Towers;
    GameData::hostilities[e].hostileTo = Teams::Martians + Teams::Environment;
    GameData::colliders[e].AABB = glm::vec3(1.8, 6.5, 1.8);
    GameData::rigidbodies[e].fixed = true;
    GameData::rigidbodies[e].grounded = false;
    GameData::AOEattackmodules[e].cooldown = 0;
    GameData::AOEattackmodules[e].fireRate = TURRET_BASE_FIRE_RATE * 2;
    GameData::AOEattackmodules[e].damage = TURRET_BASE_DMG;
    GameData::AOEattackmodules[e].range = TURRET_BASE_RANGE;
    GameData::AOEattackmodules[e].source = GameData::positions[e];

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::RigidBody +
        ComponentTags::Collidable +
        ComponentTags::AttackerAOE +
        ComponentTags::Upgradeable;
    GameData::models[e].renderCollider = true;
    GameData::colliders[e].colteam = CollisionLayer::StaticObj;
    GameData::colliders[e].colwith = 0;

    return createdEntities;
}

std::list<Entity> createTowerTesla1() {
    std::list<Entity> createdEntities;
    Entity e = createTowerTesla().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }

    GameData::models[e].modelID = MODEL_ID_TESLA;
    GameData::upgradedata[e].cost = { 20, 20, 0 };
    GameData::upgradedata[e].upgrade = Prefabs::TowerTesla2;
    GameData::AOEattackmodules[e].range *= 1.2;


    return createdEntities;
};

std::list<Entity> createTowerTesla2() {
    std::list<Entity> createdEntities;
    Entity e = createTowerTesla().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }

    GameData::models[e].modelID = MODEL_ID_TESLA;
    GameData::upgradedata[e].cost = { 100, 100, 0 };
    GameData::upgradedata[e].upgrade = Prefabs::TowerTesla3;
    GameData::AOEattackmodules[e].range *= 2; 


    return createdEntities;
};

std::list<Entity> createTowerTesla3() {
    std::list<Entity> createdEntities;
    Entity e = createTowerTesla().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }

    GameData::models[e].modelID = MODEL_ID_TESLA;
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::RigidBody +
        ComponentTags::Collidable +
        ComponentTags::AttackerAOE;
    GameData::AOEattackmodules[e].range *= 5;


    return createdEntities;
};


std::list<Entity> createTowerBarrier()
{
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::models[e].modelID = MODEL_ID_BARRIER;
    GameData::models[e].asciiRep = 'T';
    GameData::hostilities[e].team = Teams::Towers;
    GameData::hostilities[e].hostileTo = 0;
    GameData::colliders[e].AABB = glm::vec3(2, 2, 5);
    GameData::rigidbodies[e].fixed = true;
    GameData::rigidbodies[e].grounded = false;
    GameData::healths[e].curHealth = GameData::healths[e].maxHealth = BARRIER_BASE_HEALTH;
    GameData::upgradedata[e].cost = { 0, 0, 0 };
    GameData::upgradedata[e].upgrade = Prefabs::TowerBarrier1;
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::RigidBody +
        ComponentTags::Collidable +
        ComponentTags::Health +
        ComponentTags::Upgradeable;
    GameData::models[e].renderCollider = true;
    GameData::colliders[e].colteam = CollisionLayer::StaticObj;
    GameData::colliders[e].colwith = 0;

    return createdEntities;
};

std::list<Entity> createTowerBarrier1() {
    std::list<Entity> createdEntities;
    Entity e = createTowerBarrier().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }

    GameData::models[e].modelID = MODEL_ID_BARRIER;
    GameData::upgradedata[e].cost = { 20, 20, 0 };
    GameData::upgradedata[e].upgrade = Prefabs::TowerBarrier2;
    GameData::healths[e].curHealth = GameData::healths[e].maxHealth *= 2;


    return createdEntities;
};

std::list<Entity> createTowerBarrier2() {
    std::list<Entity> createdEntities;
    Entity e = createTowerBarrier().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }

    GameData::models[e].modelID = MODEL_ID_BARRIER;
    GameData::upgradedata[e].cost = { 100, 100, 0 };
    GameData::upgradedata[e].upgrade = Prefabs::TowerBarrier3;
    GameData::healths[e].curHealth = GameData::healths[e].maxHealth *= 5;


    return createdEntities;
};

std::list<Entity> createTowerBarrier3() {
    std::list<Entity> createdEntities;
    Entity e = createTowerBarrier().front();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }

    GameData::models[e].modelID = MODEL_ID_BARRIER;
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Hostility +
        ComponentTags::RigidBody +
        ComponentTags::Collidable +
        ComponentTags::Health;

    GameData::healths[e].curHealth = GameData::healths[e].maxHealth *= 10;


    return createdEntities;
};


std::list<Entity> createHome() {
    std::list<Entity> createdEntities;
    Entity e = createEntity(BASE_START, BASE_END);
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::positions[e] = baseLoc;
    GameData::models[e].asciiRep = 'B';
    GameData::models[e].modelID = MODEL_ID_BASE;
    GameData::hostilities[e].team = Teams::Towers;
    GameData::hostilities[e].hostileTo = Teams::Martians;
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = HOME_BASE_HEALTH;
    GameData::colliders[e].AABB =  glm::vec3(60, 40, 20);
    GameData::rigidbodies[e].fixed = true;
    GameData::models[e].renderCollider = true;
    GameData::rigidbodies[e].grounded = false;
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Turret +
        ComponentTags::Hostility +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::Collidable;
    GameData::colliders[e].colteam = CollisionLayer::StaticObj;
    GameData::colliders[e].colwith = 0;

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
        GameData::positions[e] = PlayerSpawns::spawnpoint[i];
        GameData::velocities[e].velocity = glm::vec3(0, 0, 0);
        GameData::velocities[e].moveSpeed = PLAYER_MVSPD_PERSEC;
        GameData::colliders[e].AABB =  glm::vec3(1, 0.7, 1);
        GameData::models[e].modelID = MODEL_ID_ROVER;
        GameData::models[e].asciiRep = 'P';
        GameData::models[i].renderCollider = true;
        GameData::healths[e].maxHealth = GameData::healths[i].curHealth = PLAYER_BASE_HEALTH;
        GameData::hostilities[e].team = Teams::Players;
        GameData::hostilities[e].hostileTo = Teams::Environment + Teams::Martians;
        GameData::pattackmodules[e].attack = Prefabs::ProjectileBasic;
        GameData::pattackmodules[e].targetPos = glm::vec3(0, 0, 0);
        GameData::pattackmodules[e].cooldown = 0;
        GameData::states[e] = 0;
        GameData::retplaces[e].buildingPrefab = Prefabs::TowerBasic;
        GameData::retplaces[e].reticlePrefab = Prefabs::TowerReticleBasic;
        GameData::retplaces[e].reticle = INVALID_ENTITY;
        GameData::retplaces[e].place = false;
        GameData::retplaces[e].validTarget = false;
        GameData::colliders[e].colteam = CollisionLayer::WorldObj;
        GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj + CollisionLayer::Boss;
        GameData::playerdata.spawntimers[e] = -2;
        Collision::updateColTable(e);

        GameData::tags[e] =
            ComponentTags::Position +
            ComponentTags::Velocity +
            ComponentTags::Model +
            ComponentTags::Collidable +
            ComponentTags::RigidBody +
            ComponentTags::Health +
            ComponentTags::Hostility;
    }
    return createdEntities;
}

std::list<Entity> createBossHead()
{
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::velocities[e].moveSpeed = ENEMY_GND_BASE_MVSPD;
    GameData::velocities[e].flying = false;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e].AABB = glm::vec3(4, 2, 2);
    GameData::models[e].modelID = MODEL_ID_BEAR_HEAD;
    GameData::models[e].asciiRep = 'E';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH;
    GameData::coldmg[e].damage = ENEMY_GND_BASE_DMG;
    GameData::hostilities[e].team = Teams::Martians;
    GameData::hostilities[e].hostileTo = Teams::Players;
    GameData::colliders[e].colteam = CollisionLayer::Boss;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj;
    GameData::resources[e].resources[ResourceType::Money] = 20;
    GameData::pointvalues[e] = 20;
    GameData::models[e].renderCollider = true;
    GameData::models[e].modelOrientation = -90;
    GameData::rigidbodies[e].fixed = false;
    GameData::rigidbodies[e].grounded = false;
    GameData::coldmg[e].cooldown = 0;
    GameData::coldmg[e].damageRate = ENEMY_BASE_DAMAGE_RATE;
    GameData::pattackmodules[e].attack = Prefabs::ProjectileChaos;
    GameData::turrets[e].attackState = ComponentTags::AttackerProjectile;
    GameData::turrets[e].range = 50;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::WorthPoints +
        ComponentTags::ResourceContainer +
        ComponentTags::HomingData +
        ComponentTags::Turret;

    return createdEntities;
}

std::list<Entity> createBossLArm()
{
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::velocities[e].moveSpeed = ENEMY_GND_BASE_MVSPD * 1.2;
    GameData::velocities[e].flying = false;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e].AABB = glm::vec3(2, 3.5, 2.5);
    GameData::models[e].modelID = MODEL_ID_BEAR_LARM;
    GameData::models[e].asciiRep = 'E';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH;
    GameData::coldmg[e].damage = ENEMY_GND_BASE_DMG;
    GameData::hostilities[e].team = Teams::Martians;
    GameData::hostilities[e].hostileTo = Teams::Players + Teams::Towers + Teams::Environment;
    GameData::colliders[e].colteam = CollisionLayer::Boss;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj;
    GameData::resources[e].resources[ResourceType::Money] = 20;
    GameData::pointvalues[e] = 20;
    GameData::models[e].renderCollider = true;
    GameData::rigidbodies[e].fixed = false;
    GameData::rigidbodies[e].grounded = false;
    GameData::coldmg[e].cooldown = 0;
    GameData::coldmg[e].damageRate = ENEMY_BASE_DAMAGE_RATE;
    GameData::pattackmodules[e].attack = Prefabs::ProjectileBasic;
    GameData::turrets[e].attackState = ComponentTags::AttackerProjectile;
    GameData::turrets[e].range = ATTACK_RANGE;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::WorthPoints +
        ComponentTags::ResourceContainer +
        ComponentTags::HomingData +
        ComponentTags::Turret;

    return createdEntities;
}

std::list<Entity> createBossBody()
{
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::states[e] = enemyState::Pathing;
    GameData::pathStructs[e].currentNode = 0;
    GameData::pathStructs[e].path = Paths::bossPath;
    GameData::velocities[e].moveSpeed = ENEMY_GND_BASE_MVSPD * 0.8;
    GameData::velocities[e].flying = false;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e].AABB = glm::vec3(5, 7, 3);
    GameData::models[e].modelID = MODEL_ID_BEAR_BODY;
    GameData::models[e].asciiRep = 'E';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH*20;
    GameData::coldmg[e].damage = ENEMY_GND_BASE_DMG;
    GameData::hostilities[e].team = Teams::Martians;
    GameData::hostilities[e].hostileTo = Teams::Players + Teams::Towers + Teams::Environment;
    GameData::colliders[e].colteam = CollisionLayer::Boss;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj;
    GameData::resources[e].resources[ResourceType::Money] = 20;
    GameData::pointvalues[e] = 20;
    GameData::models[e].renderCollider = true;
    GameData::rigidbodies[e].fixed = false;
    GameData::rigidbodies[e].grounded = false;
    GameData::coldmg[e].cooldown = 0;
    GameData::coldmg[e].damageRate = ENEMY_BASE_DAMAGE_RATE;


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
}

std::list<Entity> createBossRArm()
{
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::velocities[e].moveSpeed = ENEMY_GND_BASE_MVSPD * 1.2;
    GameData::velocities[e].flying = false;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e].AABB = glm::vec3(2, 2, 3);
    GameData::models[e].modelID = MODEL_ID_BEAR_RARM;
    GameData::models[e].asciiRep = 'E';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH;
    GameData::coldmg[e].damage = ENEMY_GND_BASE_DMG;
    GameData::hostilities[e].team = Teams::Martians;
    GameData::hostilities[e].hostileTo = Teams::Players + Teams::Towers + Teams::Environment;
    GameData::colliders[e].colteam = CollisionLayer::Boss;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj;
    GameData::resources[e].resources[ResourceType::Money] = 20;
    GameData::pointvalues[e] = 20;
    GameData::models[e].renderCollider = true;
    GameData::rigidbodies[e].fixed = false;
    GameData::rigidbodies[e].grounded = false;
    GameData::coldmg[e].cooldown = 0;
    GameData::coldmg[e].damageRate = ENEMY_BASE_DAMAGE_RATE;
    GameData::pattackmodules[e].attack = Prefabs::ProjectileSpray;
    GameData::turrets[e].attackState = ComponentTags::AttackerProjectile;
    GameData::turrets[e].range = ATTACK_RANGE;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::WorthPoints +
        ComponentTags::ResourceContainer +
        ComponentTags::HomingData +
        ComponentTags::Turret;

    return createdEntities;
}

std::list<Entity> createBossRLeg()
{
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::velocities[e].moveSpeed = ENEMY_GND_BASE_MVSPD;
    GameData::velocities[e].flying = false;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e].AABB = glm::vec3(2, 2.5, 3);
    GameData::models[e].modelID = MODEL_ID_BEAR_RLEG;
    GameData::models[e].asciiRep = 'E';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH;
    GameData::coldmg[e].damage = ENEMY_GND_BASE_DMG;
    GameData::hostilities[e].team = Teams::Martians;
    GameData::hostilities[e].hostileTo = Teams::Players + Teams::Towers + Teams::Environment;
    GameData::colliders[e].colteam = CollisionLayer::Boss;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj;
    GameData::resources[e].resources[ResourceType::Money] = 20;
    GameData::pointvalues[e] = 20;
    GameData::models[e].renderCollider = true;
    GameData::rigidbodies[e].fixed = false;
    GameData::rigidbodies[e].grounded = false;
    GameData::coldmg[e].cooldown = 0;
    GameData::AOEattackmodules[e].fireRate = STOMP_RATE;
    GameData::AOEattackmodules[e].damage = STOMP_DMG;
    GameData::AOEattackmodules[e].range = STOMP_RANGE;

    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::WorthPoints +
        ComponentTags::ResourceContainer +
        ComponentTags::HomingData +
        ComponentTags::AttackerAOE;

    return createdEntities;
}

std::list<Entity> createBossLLeg()
{
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::velocities[e].moveSpeed = ENEMY_GND_BASE_MVSPD;
    GameData::velocities[e].flying = false;
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e].AABB = glm::vec3(2, 2, 4);
    GameData::models[e].modelID = MODEL_ID_BEAR_LLEG;
    GameData::models[e].asciiRep = 'E';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH;
    GameData::coldmg[e].damage = ENEMY_GND_BASE_DMG;
    GameData::hostilities[e].team = Teams::Martians;
    GameData::hostilities[e].hostileTo = Teams::Players + Teams::Towers + Teams::Environment;
    GameData::colliders[e].colteam = CollisionLayer::Boss;
    GameData::colliders[e].colwith = CollisionLayer::WorldObj + CollisionLayer::StaticObj;
    GameData::resources[e].resources[ResourceType::Money] = 20;
    GameData::pointvalues[e] = 20;
    GameData::models[e].renderCollider = true;
    GameData::rigidbodies[e].fixed = false;
    GameData::rigidbodies[e].grounded = false;
    GameData::coldmg[e].cooldown = 0;
    GameData::coldmg[e].damageRate = ENEMY_BASE_DAMAGE_RATE;
    GameData::AOEattackmodules[e].fireRate = STOMP_RATE;
    GameData::AOEattackmodules[e].damage = STOMP_DMG;
    GameData::AOEattackmodules[e].range = STOMP_RANGE;
    GameData::tags[e] =
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::WorthPoints +
        ComponentTags::ResourceContainer +
        ComponentTags::HomingData +
        ComponentTags::AttackerAOE;

    return createdEntities;
}

std::list<Entity> createBoss()
{
    std::list<Entity> boss;
    Entity body = createBossBody().front();
    Entity head = createBossHead().front();
   
    Entity lleg = createBossLLeg().front();
    Entity rleg = createBossRLeg().front();
    Entity larm = createBossLArm().front();
    Entity rarm = createBossRArm().front();
    boss.push_back(body);
    boss.push_back(head);
    boss.push_back(lleg);
    boss.push_back(rleg);
    boss.push_back(larm);
    boss.push_back(rarm);

    GameData::homingStructs[head].trackedEntity = body;
    GameData::homingStructs[head].offset = glm::vec3(0, GameData::colliders[body].AABB.y + GameData::colliders[head].AABB.y, 0);
  
    
    GameData::homingStructs[lleg].trackedEntity = body;
    GameData::homingStructs[lleg].offset = glm::vec3(-GameData::colliders[body].AABB.x - GameData::colliders[lleg].AABB.x + 2, -GameData::colliders[body].AABB.y + GameData::colliders[lleg].AABB.y, 2);
    GameData::homingStructs[rleg].trackedEntity = body;
    GameData::homingStructs[rleg].offset = glm::vec3(GameData::colliders[body].AABB.x + GameData::colliders[rleg].AABB.x - 2, -GameData::colliders[body].AABB.y + GameData::colliders[rleg].AABB.y, 2);
    GameData::homingStructs[larm].trackedEntity = body;
    GameData::homingStructs[larm].offset = glm::vec3(-GameData::colliders[body].AABB.x - GameData::colliders[larm].AABB.x + 2, GameData::colliders[body].AABB.y - GameData::colliders[larm].AABB.y - 2, 3);
    GameData::homingStructs[rarm].trackedEntity = body;
    GameData::homingStructs[rarm].offset = glm::vec3(GameData::colliders[body].AABB.x + GameData::colliders[rarm].AABB.x - 2, GameData::colliders[body].AABB.y - GameData::colliders[rarm].AABB.y - 3, 2);
    
    //GameData::positions[body] = glm::vec3(0, 0, -60);
    GameData::positions[head] = glm::vec3(0, 0, -70);
    
    GameData::positions[lleg] = glm::vec3(-20, 0, -70);

    GameData::positions[rleg] = glm::vec3(20, 0, -70);

    GameData::positions[larm] = glm::vec3(-40, 0, -70);

    GameData::positions[rarm] = glm::vec3(40, 0, -70);
    



    return boss;
}

std::list<Entity> createWoodResourceBasic()
{
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e].AABB =  glm::vec3(2, 8, 2);
    GameData::models[e].asciiRep = 'R';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = RESOURCE_BASE_HEALTH;
    GameData::hostilities[e].team = Teams::Environment;
    GameData::models[e].modelID = MODEL_ID_RESOURCE;
    GameData::hostilities[e].hostileTo = 0;
    GameData::colliders[e].colteam = CollisionLayer::StaticObj;
    GameData::colliders[e].colwith = 0;
    GameData::resources[e].resources[ResourceType::Wood] = 20;
    GameData::rigidbodies[e].fixed = true;
    GameData::models[e].renderCollider = true;

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
    GameData::positions[e] = glm::vec3(0, 0, 0);
    GameData::colliders[e].AABB =  glm::vec3(1, 1, 1);
    GameData::models[e].asciiRep = 'R';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = RESOURCE_BASE_HEALTH;
    GameData::hostilities[e].team = Teams::Environment;
    GameData::hostilities[e].hostileTo = 0;
    GameData::models[e].modelID = MODEL_ID_RESOURCE_STONE;
    GameData::colliders[e].colteam = CollisionLayer::StaticObj;
    GameData::colliders[e].colwith = 0;
    GameData::resources[e].resources[ResourceType::Stone] = 20;
    GameData::rigidbodies[e].fixed = true;
    GameData::models[e].renderCollider = true;

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


namespace PlayerSpawns {
    const glm::vec3 spawnpoint[numSpawns] = {
        glm::vec3(-20,0,50), glm::vec3(-10,0,50), glm::vec3(0,0,50), glm::vec3(10,0,50)
    };
}\
namespace Paths {
    const glm::vec3 path[pathCount][PATH_LENGTH] =
    {
        { glm::vec3(-70,0,-85),glm::vec3(-70,0,-45), glm::vec3(-50,0,-45),glm::vec3(-50,0,-5),glm::vec3(-70,0,-5), glm::vec3(-70,0,35), glm::vec3(-50,0,35), glm::vec3(-50,0,85), glm::vec3(-50,0,85)},
        { glm::vec3(-40,0,-85), glm::vec3(-40,0,-45), glm::vec3(-20,0,-45),glm::vec3(-20,0,-5), glm::vec3(-40,0,-5),glm::vec3(-40,0,35), glm::vec3(-20,0,35), glm::vec3(-20,0,85), glm::vec3(-20,0,85) },
        { glm::vec3(0,0,-85), glm::vec3(0,0,85), glm::vec3(0,0,85), baseLoc, baseLoc, baseLoc, baseLoc, baseLoc  },
        { glm::vec3(40,0,-85), glm::vec3(40,0,-45), glm::vec3(20,0,-45),glm::vec3(20,0,-5), glm::vec3(40,0,-5),glm::vec3(40,0,35), glm::vec3(20,0,35), glm::vec3(20,0,85), glm::vec3(20,0,85) },
        { glm::vec3(70,0,-85),glm::vec3(70,0,-45), glm::vec3(50,0,-45),glm::vec3(50,0,-5),glm::vec3(70,0,-5), glm::vec3(70,0,35), glm::vec3(50,0,35), glm::vec3(50,0,85), glm::vec3(50,0,85)},
    };
    std::list<Entity> pathlist;
}
namespace WaveData {
    int currentWave;

    // How much time a wave lasts before the next wave starts spawning (timer for last wave will be time until win)
    int waveTimers[WAVE_COUNT] = { 60 * TICK_RATE, 60 * TICK_RATE, 60 * TICK_RATE, 60 * TICK_RATE, 90  * TICK_RATE, 90 * TICK_RATE, 120 * TICK_RATE };

    int waveTick; //countdown timer for waves

    int enemyTypes[NUM_ENEMY_TYPES] = { Prefabs::EnemyGroundBasic, Prefabs::EnemyGroundTank, Prefabs::EnemyFlyingBasic, Prefabs::EnemyFlyingTractor, Prefabs::EnemyGroundMini};

    std::queue<enemy> waves[WAVE_COUNT];
}

//Define Tower Build costs
const std::array<std::array<int, NUM_RESOURCE_TYPES>, NUM_TOWER_TYPES> buildcosts =
{ {
    {{20, 40, 0}},
    {{40, 20, 40}},
    {{40, 40, 20}},
    {{20, 0, 40}}
}};

std::list<Entity> createPathColliders()
{
    std::list<Entity> createdEntities;
    for (int p = 0; p < Paths::pathCount; ++p) {
        glm::vec3 pathvecprev = glm::vec3(0, 0, 0);
        for (int i = 0; (i < PATH_LENGTH - 1); i++) {
            if (Paths::path[p][i] == Paths::path[p][i + 1]) {
                break;
            }
            float currdiff = glm::distance(Paths::path[p][i + 1], Paths::path[p][i]);
            float progress = 0;
            glm::vec3 pathvec = glm::normalize(Paths::path[p][i + 1] - Paths::path[p][i]);
            //printf("Curdiif is init %f\n", currdiff);

            //Do first one, which could be a corner
            Entity e = createEntity();
            createdEntities.push_back(e);
            if (e == INVALID_ENTITY) {
                return createdEntities;
            }

            //printf("Curdiif is now %f creating entity %d\n", currdiff, e);


            GameData::activity[e] = true;

            GameData::positions[e] = Paths::path[p][i] + progress * pathvec;
            GameData::positions[e].y = GROUND_HEIGHT + 0.01;

            GameData::colliders[e].AABB = glm::vec3(PATH_WIDTH / 2, 0.01, PATH_WIDTH / 2);

            GameData::models[e].modelID = MODEL_ID_PATH_STRAIGHT;
            GameData::models[e].asciiRep = 'P';
            GameData::models[e].renderCollider = true;
            GameData::models[e].modelOrientation = glm::degrees(glm::acos(pathvec.x));
            GameData::colliders[e].colteam = CollisionLayer::UIObj;
            GameData::colliders[e].colwith = 0;

            GameData::tags[e] =
                ComponentTags::Position +
                ComponentTags::Model +
                ComponentTags::Collidable;

            if (glm::dot(pathvecprev, pathvec) != 0) {
                //printf("New path is in sam direction as old\n");
            }
            else if ((pathvecprev + pathvec) == glm::vec3(1, 0, 1)) {
                GameData::models[e].modelID = MODEL_ID_PATH_CORNER;
                GameData::models[e].modelOrientation = 180;
            }
            else if ((pathvecprev + pathvec) == glm::vec3(-1, 0, 1)) {
                GameData::models[e].modelID = MODEL_ID_PATH_CORNER;
                GameData::models[e].modelOrientation = 270;
            }
            else if ((pathvecprev + pathvec) == glm::vec3(-1, 0, -1)) {
                GameData::models[e].modelID = MODEL_ID_PATH_CORNER;
                GameData::models[e].modelOrientation = 0;
            }
            else if ((pathvecprev + pathvec) == glm::vec3(1, 0, -1)) {
                GameData::models[e].modelID = MODEL_ID_PATH_CORNER;
                GameData::models[e].modelOrientation = 90;
            }

            progress += PATH_WIDTH;

            while (currdiff > progress) {

                Entity e = createEntity();
                createdEntities.push_back(e);
                if (e == INVALID_ENTITY) {
                    return createdEntities;
                }

                //printf("Curdiif is now %f creating entity %d\n", currdiff, e);


                GameData::activity[e] = true;

                GameData::positions[e] = Paths::path[p][i] + progress*pathvec;
                GameData::positions[e].y = GROUND_HEIGHT + 0.01;
                
                GameData::colliders[e].AABB = glm::vec3(PATH_WIDTH / 2, 0.01 , PATH_WIDTH / 2);

                GameData::models[e].modelID = MODEL_ID_PATH_STRAIGHT;
                GameData::models[e].asciiRep = 'P';
                GameData::models[e].renderCollider = true;
                GameData::models[e].modelOrientation = glm::degrees(glm::acos(pathvec.x));
                GameData::colliders[e].colteam = CollisionLayer::UIObj;
                GameData::colliders[e].colwith = 0;

                GameData::tags[e] =
                    ComponentTags::Position +
                    ComponentTags::Model +
                    ComponentTags::Collidable;

                progress += PATH_WIDTH;


            }
            pathvecprev = pathvec*-1.0f;
        }
    }

    return createdEntities;
};
