#include "Prefabs.h"

void Prefabs::enemy_ground_basic(Entity e) 
{
    //Assign Tags
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

    GameData::activity[e] = true;
    GameData::pathStructs[e].currentNode = 0;
    GameData::pathStructs[e].moveSpeed = ENEMY_GND_BASE_MVSPD;
    GameData::colliders[e] = { glm::vec3(1, 1, 1) };
    GameData::models[e].asciiRep = 'E';
    GameData::healths[e].maxHealth = GameData::healths[e].curHealth = ENEMY_BASE_HEALTH;
    GameData::coldmg[e].damage = ENEMEY_GND_BASE_DMG;
    GameData::hostilities[e].team = Teams::Martians;
    GameData::hostilities[e].hostileTo = Teams::Players + Teams::Towers;
}
