#include "EntityComponentSystem.h"

namespace GameData
{
  std::array<Tag, MAX_ENTITIES> tags;

  std::array<Active, MAX_ENTITIES> activity;
  std::array<Position, MAX_ENTITIES> positions;
  std::array<Velocity, MAX_ENTITIES> velocities;
  std::array<PathData, MAX_ENTITIES> pathStructs;
  std::array<Model, MAX_ENTITIES> models;
  std::array<Turret, MAX_ENTITIES> turrets;
  std::array<Collider, MAX_ENTITIES> colliders;
  std::queue<CollisionEvent> colevents;
  std::array<RigidBodyInfo, MAX_ENTITIES> rigidbodies;
  std::array<Health, MAX_ENTITIES> healths;
  std::array<CollisionDmg, MAX_ENTITIES> coldmg;
  std::array<Hostility, MAX_ENTITIES> hostilities;
  std::array<LifeSpan, MAX_ENTITIES> lifespans;
  std::array<AttackModule, MAX_ENTITIES> attackmodules;
}

//Call all systems each update
void EntityComponentSystem::update()
{
    sysAttacks();
    sysPathing();
    sysMovement();
    sysTurretFire();
    sysHealthStatus();
    sysDetectCollisions();
    resolveCollisions();
    sysLifeSpan();
}

//Move Entities that contain a Velocity Component
void EntityComponentSystem::sysMovement()
{
    for (Entity e = 0; e < MAX_ENTITIES; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }

        //check if this entity has a velocity component to update its position
        if ((GameData::tags[e] & ComponentTags::Velocity) == ComponentTags::Velocity)
        {
            GameData::positions[e] = GameData::positions[e] + GameData::velocities[e];
        }
    }
}

//TODO: FINISH PATHING ALGO
//Do pathfinding for entities with PathData components
void EntityComponentSystem::sysPathing()
{
    for (Entity e = 0; e < MAX_ENTITIES; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }

        //check if this entity has a PathData component to do pathing
        if ((GameData::tags[e] & ComponentTags::PathData) == ComponentTags::PathData)
        {
            //Check if entity has reached its currently tracked destination (+/- 1 unit)
            glm::vec3 nodePos = GameData::pathStructs[e].pathNodes[GameData::pathStructs[e].currentNode];
            bool closeEnough = glm::distance(nodePos, GameData::positions[e]) < 1;
            //bool closeEnough = glm::floor(GameData::positions[e]) == glm::floor(nodePos);
            if (closeEnough)
            {
                //Node reached, increment current Node
                //std::cout << "Entity " << e << " Reached Node " << GameData::pathStructs[e].currentNode << "\n";
                GameData::pathStructs[e].currentNode++;
            }

            //Check if entity has reached the final pathNode (currentNode now out of bounds)
            if (GameData::pathStructs[e].currentNode >= PATH_LENGTH)
            {
                //TODO: (Temporarily Set Enemy to inactive once it reaches the end of the path)
                GameData::activity[e] = false;
                //std::cout << "Entity " << e << " Reached the end of its path! (Despawning...)\n";
                continue;
            }

            //Update entity velocity vector to move towards tracked node
            glm::vec3 direction = GameData::pathStructs[e].pathNodes[GameData::pathStructs[e].currentNode] - GameData::positions[e];
            GameData::velocities[e] = glm::normalize(direction) * GameData::pathStructs[e].moveSpeed;
        }
    }
}

void EntityComponentSystem::sysTurretFire()
{
    for (Entity e = 0; e < MAX_ENTITIES; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }
        //check if this entity has a turret component
        if ((GameData::tags[e] & ComponentTags::Turret) == ComponentTags::Turret)
        {
            //Find closest enemy to shoot
            Entity closestEnemy = e; //initialized to turret ID in case of no valid target found
            float closestDistance = GameData::turrets[e].range + 1; //Set closest found distance to out of range
            //Loop Thru enemies and find one in range
            for (Entity i = 0; i < MAX_ENTITIES; i++)
            {
                //Check if enemy is active
                if (!GameData::activity[i]) { continue; }
                //Check if hostileto
                if (!(GameData::hostilities[e].hostileTo & GameData::hostilities[i].team)) { continue; }
                //Check if enemy is in range
                float enemyDistance = glm::distance(GameData::positions[e], GameData::positions[i]);
                if (enemyDistance < GameData::turrets[e].range)
                {
                    //check if this enemy is the new closest entity
                    if (enemyDistance < closestDistance)
                    {
                        //Set Orientation in model to new orientation
                        GameData::models[e].dirNorm = glm::normalize(GameData::positions[i] - GameData::positions[e]);

                        closestEnemy = i;
                        closestDistance = enemyDistance;
                    }
                }
            }

            //If a valid target was found, fire at them
            if (closestEnemy != e)
            {
                GameData::healths[closestEnemy].curHealth -= (GameData::turrets[e].damage);
                //std::cout << "Test Tower Fired at Enemey: " << closestEnemy - ENEMY_START << "\n";
            }
        }
    }
}

void EntityComponentSystem::sysDetectCollisions()
{
    for (Entity e = 0; e < MAX_ENTITIES; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }
        Tag collides = ComponentTags::Position + ComponentTags::Collidable;
        //check if this entity can can collide
        if ((GameData::tags[e] & collides) == collides)
        {

            //Check collisions with everything else
            for (Entity o = 0; o < MAX_ENTITIES; o++)
            {
                //Continue to next entity if this one is not active
                if (!GameData::activity[o]) { continue; }


                //Continue to next entity if this one is itself
                if (o == e) { continue; }

                Tag collides = ComponentTags::Position + ComponentTags::Collidable;
                //check if this entity has can collide
                if ((GameData::tags[o] & collides) == collides)
                {
                    glm::vec3 maxe = GameData::positions[e] + GameData::colliders[e].AABB;
                    glm::vec3 maxo = GameData::positions[o] + GameData::colliders[o].AABB;
                    glm::vec3 mine = GameData::positions[e] - GameData::colliders[e].AABB;
                    glm::vec3 mino = GameData::positions[o] - GameData::colliders[o].AABB;
                    if (glm::all(glm::lessThan(mine, maxo)) && glm::all(glm::lessThan(mino, maxe))) {
                        glm::vec3 pen = glm::vec3(0);
                        glm::vec3 diff1 = maxo - mine;
                        glm::vec3 diff2 = maxe - mino;
                        float min = 10000;
                        int index = 0;
                        if (diff1.x < min) {
                            min = diff1.x;
                            index = 0;
                        }
                        if (diff1.y < min) {
                            min = diff1.y;
                            index =1;
                        }
                        if (diff1.z < min) {
                            min = diff1.z;
                            index = 2;
                        }
                        if (diff2.x < min) {
                            min = diff2.x;
                            index = 3;
                        }
                        if (diff2.y < min) {
                            min = diff2.y;
                            index = 4;
                        }
                        if (diff2.z < min) {
                            min = diff2.z;
                            index = 5;
                        }
                        if (index < 3) {
                            pen[index] = diff1[index];
                        }
                        else {
                            pen[index-3] = -1*diff2[index-3];
                        }

                        //Create Collision Event objects in e
                        GameData::colevents.push(CollisionEvent{ e, o, pen });

                        //printf("Diff1: %f, %f, %f,  Diff2: %f, %f, %f\n Index: %d, Minn: %f\n", diff1.x, diff1.y, diff1.z, diff2.x, diff2.y, diff2.z, index, min);
                    }
                }
            }
        }
    }
}

void EntityComponentSystem::resolveCollisions()
{
    while (!GameData::colevents.empty())
    {
        CollisionEvent ce = GameData::colevents.front();
        GameData::colevents.pop();
        Entity e = ce.e;
        Entity o = ce.o;
        //Rigid Body Collision
        if (((GameData::tags[e] & (ComponentTags::RigidBody)) == ComponentTags::RigidBody)&& (GameData::tags[o] & (ComponentTags::RigidBody)) == ComponentTags::RigidBody)
        {
            if (!GameData::rigidbodies[e].fixed) {
                GameData::positions[e] += ce.pen;
            }

        }



        //Check if dies on Collision
        if ((GameData::tags[e] & (ComponentTags::DiesOnCollision)) == ComponentTags::DiesOnCollision) {
            GameData::activity[e] = false;
        }

        //Do on collision damage
        if ((GameData::tags[e] & (ComponentTags::CollisionDmg)) == ComponentTags::CollisionDmg) {
            //Check if hostileto
            if ((GameData::hostilities[e].hostileTo & GameData::hostilities[o].team)) {
                if ((GameData::tags[o] & (ComponentTags::Health)) == ComponentTags::Health) {
                    GameData::healths[o].curHealth -= GameData::coldmg[e].damage;
                }
            }
        }
        if ((GameData::tags[o] & (ComponentTags::CollisionDmg)) == ComponentTags::CollisionDmg) {
            //Check if hostileto
            if ((GameData::hostilities[o].hostileTo & GameData::hostilities[e].team)) {
                if ((GameData::tags[e] & (ComponentTags::Health)) == ComponentTags::Health) {
                    GameData::healths[e].curHealth -= GameData::coldmg[o].damage;
                }
            }
        }




    }
}

//Check entity health death conditions
void EntityComponentSystem::sysHealthStatus()
{
    for (Entity e = 0; e < MAX_ENTITIES; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }

        //check if this entity has a health component
        if ((GameData::tags[e] & ComponentTags::Health) == ComponentTags::Health)
        {
            //TEST OUTPUT FOR VISUALIZER
            if (GameData::healths[e].curHealth <= 10)
            {
                GameData::models[e].asciiRep = 'X';
            }

            //set to inactive if health at or below 0
            if (GameData::healths[e].curHealth <= 0)
            {
                GameData::activity[e] = false;
                //std::cout << "Enemy: " << e - ENEMY_START << " Died\n";
            }
        }
    }
}

void EntityComponentSystem::sysAttacks()
{

    for (int e = 0; e < MAX_ENTITIES; ++e) {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }

        if ((GameData::tags[e] & ComponentTags::Attacker) == ComponentTags::Attacker) {

            if (GameData::attackmodules[e].isAttacking) {
                //printf("Attacker %u is attacking, checking cooldown\n", e);
                if (GameData::attackmodules[e].cooldown <= 0) {
                    printf("Attacker %u is attacking, creating projectile %d\n", e, GameData::attackmodules[e].cooldown);
                    Entity attack = createProjectile(); //TODO: Can add support for multi entity attacks where create returns a list
                    if (attack == INVALID_ENTITY) {
                        printf("Invalid Entity (ran of of projectiles)\n");
                        continue;
                    }
                    //Create transformation matrix from prefab dim, to attacker dim
                    glm::vec3 normRelTarget = glm::normalize(GameData::attackmodules[e].targetPos - GameData::positions[e]);
                    float angle = glm::acos(-normRelTarget.z);
                    glm::vec3 axis = glm::vec3(normRelTarget.y, -normRelTarget.x, 0);
                    glm::mat4 transform = glm::translate(GameData::positions[e]) * glm::rotate(angle, axis);

                    //Transform positions and velocity relative to attacker
                    GameData::positions[attack] = transform * glm::vec4(GameData::positions[attack], 1);
                    GameData::velocities[attack] = transform * glm::vec4(GameData::velocities[attack], 0);
                    //Set Hostility
                    GameData::hostilities[attack].team = GameData::hostilities[e].team;
                    GameData::hostilities[attack].hostileTo = GameData::hostilities[e].hostileTo;

                    //Set cooldown TODO, should be its own component in an attack
                    GameData::attackmodules[e].cooldown = 64;
                }
               
            }
            GameData::attackmodules[e].cooldown--;
        }
    }
}

void EntityComponentSystem::sysLifeSpan()
{
    for (Entity e = 0; e < MAX_ENTITIES; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }

        //check if this entity has a health component
        if ((GameData::tags[e] & ComponentTags::LifeSpan) == ComponentTags::LifeSpan)
        {
            GameData::lifespans[e] -= 1.0f / TICK_RATE;
            if (GameData::lifespans[e] <= 0) {
                GameData::activity[e] = false;
            }
        }
    }
}

//Create an empty projectile 
Entity EntityComponentSystem::createEntity()
{
    //Once we get instancing and modelID rendering setup,
    for (int i = ENEMY_START; i < ENEMY_END; ++i) {
        if (!GameData::activity[i]) {
            GameData::activity[i] = true;
            GameData::tags[i] = ComponentTags::Active;
            return i;
        }
    }
    return INVALID_ENTITY;
}

Entity EntityComponentSystem::createProjectile()
{
    Entity i = createEntity();
    if (i == INVALID_ENTITY) {
        return i;
    }
    //Create Path (TEMP FOR TESTING) TODO: REMOVE FOR FINAL VERSION
    GameData::activity[i] = true;
    GameData::positions[i] = glm::vec3(0, 0, -4);
    GameData::velocities[i] = glm::vec3(0, 0, -0.5);
    GameData::colliders[i] = { glm::vec3(1, 1, 1) };
    GameData::models[i].asciiRep = 'E';
    GameData::coldmg[i].damage = 30.0f;
    GameData::lifespans[i] = 5;

    GameData::tags[i] =
        ComponentTags::Active +
        ComponentTags::Position +
        ComponentTags::Velocity +
        ComponentTags::Model +
        ComponentTags::Collidable +
        ComponentTags::DiesOnCollision +
        ComponentTags::CollisionDmg +
        ComponentTags::Hostility +
        ComponentTags::LifeSpan;
    return i;
    return Entity();
}

glm::vec3 EntityComponentSystem::computeRaycast(glm::vec3& pos, glm::vec3& dir, float tminog, float tmaxog)
{
    printf("Cam pos: %f %f %f\n", pos.x, pos.y, pos.z);
    printf("Cam dir: %f %f %f\n", dir.x, dir.y, dir.z);

    float tfirst = 1024;
    glm::vec3 dirNorm = glm::normalize(dir);
    float tmin = tminog;
    float tmax = tmaxog;
    for (Entity e = 0; e < MAX_ENTITIES; ++e) {
        if (!GameData::activity[e]) { continue; }

        if ((GameData::tags[e] & (ComponentTags::Collidable + ComponentTags::RigidBody)) == (ComponentTags::Collidable + ComponentTags::RigidBody)) {
            glm::vec3 max = GameData::positions[e] + GameData::colliders[e].AABB;
            glm::vec3 min = GameData::positions[e] - GameData::colliders[e].AABB;
            tmin = tminog;
            tmax = tmaxog;
            bool intersect = true;
            for (int a = 0; a < 3; ++a) {
                float invD = 1.0f / dirNorm[a];
                float t0 = (min[a] - pos[a]) * invD;
                float t1 = (max[a] - pos[a]) * invD;
                if (invD < 0.0f) {
                    std::swap(t0, t1);
                }
                tmin = t0 > tmin ? t0 : tmin;
                tmax = t1 < tmax ? t1 : tmax;
                if (tmax <= tmin) {
                    intersect = false;
                }
            }
            if (intersect) {
                if (tmin < tfirst) {
                    printf("Firing at entity %d\n", e);
                    tfirst = tmin;
                }
            }
        }
    }
    if (tfirst == 1024) {
        printf("Firing at nothing\n");
    }


    return pos + (dirNorm * tfirst);
}

