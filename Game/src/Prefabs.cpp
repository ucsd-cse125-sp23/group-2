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
