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
    GameData::activity[e] = true;
    GameData::positions[e] = glm::vec3(0, 0, -4);
    GameData::velocities[e] = glm::vec3(0, 0, -0.5);
    GameData::colliders[e] = { glm::vec3(1, 1, 1) };
    GameData::models[e].asciiRep = 'J';
    GameData::coldmg[e].damage = 30.0f;
    GameData::lifespans[e] = 5;

    GameData::tags[e] =
        ComponentTags::Active +
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
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
        GameData::velocities[e] = glm::vec3((2 - i), 0, -0.5);
        GameData::colliders[e] = { glm::vec3(.25, .25, .25) };
        GameData::models[e].asciiRep = 'J';
        GameData::coldmg[e].damage = 30.0f;
        GameData::lifespans[e] = 1;

        GameData::tags[e] =
            ComponentTags::Active +
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
std::list<Entity> createEnemyGroundBasic() {
    std::list<Entity> createdEntities;
    Entity e = createEntity();
    createdEntities.push_back(e);
    if (e == INVALID_ENTITY) {
        return createdEntities;
    }
    GameData::activity[e] = true;
    GameData::pathStructs[e].currentNode = 0;
    GameData::pathStructs[e].moveSpeed = ENEMY_GND_BASE_MVSPD;
    GameData::colliders[e] = { glm::vec3(1, 1, 1) };
    GameData::models[e].asciiRep = 'E';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH;
    GameData::coldmg[e].damage = ENEMEY_GND_BASE_DMG;
    GameData::hostilities[e].team = Teams::Martians;
    GameData::hostilities[e].hostileTo = Teams::Players + Teams::Towers;

    GameData::tags[e] =
        ComponentTags::Active +
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::PathData +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::DiesOnCollision +
        ComponentTags::RigidBody +
        ComponentTags::Health +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility;

    return createdEntities;
};
namespace Paths {
    glm::vec3 path1[PATH_LENGTH] = { glm::vec3(60,0,0), glm::vec3(40,0,0), glm::vec3(40,0,20), glm::vec3(20,0,20), glm::vec3(20,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) };
    glm::vec3 path2[PATH_LENGTH] = { glm::vec3(-30,0,0), glm::vec3(-20,0,0), glm::vec3(-20,0,-10), glm::vec3(-10,0,-10), glm::vec3(-10,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) };
    glm::vec3 path3[PATH_LENGTH] = { glm::vec3(0,0,30), glm::vec3(0,0,20), glm::vec3(-10,0,20), glm::vec3(-10,0,10), glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) };
    glm::vec3 path4[PATH_LENGTH] = { glm::vec3(0,0,-30), glm::vec3(0,0,-20), glm::vec3(10,0,-20), glm::vec3(10,0,-10), glm::vec3(0,0,-10), glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) };
}
