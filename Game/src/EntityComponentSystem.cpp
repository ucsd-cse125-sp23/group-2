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
  std::array<ProjectileAttackModule, MAX_ENTITIES> pattackmodules;
  std::array<CombatLog, CLOG_MAXSIZE> combatLogs;
  std::array<Creator, MAX_ENTITIES> creators;
  std::array<SpawnRate, MAX_ENTITIES> spawnrates;
  int logpos = 0;
  std::array<State, MAX_ENTITIES> states;
  std::array<ReticlePlacement, MAX_ENTITIES> retplaces;
  Score score;
}

//Call all systems each update
void EntityComponentSystem::update()
{
    GameData::logpos = 0;
    sysDeathStatus();
    sysAttacks();
    sysPathing();
    sysGravity();
    sysMovement();
    sysTurretFire();
    sysDetectCollisions();
    resolveCollisions();
    sysBuild();
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

//Apply gravity to rigid bodies
void EntityComponentSystem::sysGravity()
{
    for (Entity e = 0; e < MAX_ENTITIES; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }

        //check if this entity has a is a rigid body
        if ((GameData::tags[e] & ComponentTags::RigidBody) == ComponentTags::RigidBody)
        {
            if (GameData::positions[e].y > 0) {
                GameData::velocities[e].y += GRAVITY;
            }
            else {
                GameData::positions[e].y = 0;
                if (GameData::velocities[e].y < 0) {
                    GameData::velocities[e].y = 0;
                }
            }
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
            glm::vec3 nodePos = Paths::path[GameData::pathStructs[e].path][GameData::pathStructs[e].currentNode];
            bool closeEnough = glm::distance(nodePos, GameData::positions[e]) < 1;
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
            glm::vec3 direction = Paths::path[GameData::pathStructs[e].path][GameData::pathStructs[e].currentNode] - GameData::positions[e];
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
                dealDamage(e, closestEnemy, (GameData::turrets[e].damage));
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
                    //If not on same  collision layer skip
                    if (!(GameData::colliders[e].colwith & GameData::colliders[o].colteam)) { continue; }
                    glm::vec3 maxe = GameData::positions[e] + GameData::colliders[e].AABB;
                    glm::vec3 maxo = GameData::positions[o] + GameData::colliders[o].AABB;
                    glm::vec3 mine = GameData::positions[e] - GameData::colliders[e].AABB;
                    glm::vec3 mino = GameData::positions[o] - GameData::colliders[o].AABB;
                    if (colCheck(e, o)) {
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
            causeDeath(e, e);
        }

        //Do on collision damage
        if ((GameData::tags[e] & (ComponentTags::CollisionDmg)) == ComponentTags::CollisionDmg) {
            //Check if hostileto
            if ((GameData::hostilities[e].hostileTo & GameData::hostilities[o].team)) {
                if ((GameData::tags[o] & (ComponentTags::Health)) == ComponentTags::Health) {
                    dealDamage(e, o, GameData::coldmg[e].damage);
                }
            }
        }




    }
}

//Check entity death flag and set to zero (and do any on death effects
void EntityComponentSystem::sysDeathStatus()
{
    for (Entity e = 0; e < MAX_ENTITIES; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }

        //set to inactive if dying flag
        if ((GameData::tags[e] & ComponentTags::Dead) == ComponentTags::Dead)
        {
            //TEST OUTPUT FOR VISUALIZER
            GameData::models[e].asciiRep = 'X';
            GameData::activity[e] = false;
            //std::cout << "Enemy: " << e - ENEMY_START << " Died\n";
        }

    }
}

void EntityComponentSystem::sysAttacks()
{

    for (int e = 0; e < MAX_ENTITIES; ++e) {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }
        GameData::pattackmodules[e].cooldown -= 1.0f / TICK_RATE;

        if ((GameData::tags[e] & ComponentTags::Attacker) == ComponentTags::Attacker) {
            //printf("Attacker %u is attacking, checking cooldown\n", e);
            if (GameData::pattackmodules[e].cooldown <= 0) {
                //printf("Attacker %u is attacking, creating projectile %d\n", e, GameData::pattackmodules[e].cooldown);
                //Create transformation matrix from prefab dim, to attacker dim
                glm::vec3 targetVec = GameData::pattackmodules[e].targetPos - GameData::positions[e];

                glm::vec3 normXZ = glm::normalize(glm::vec3(targetVec.x, 0, targetVec.z));
                float angleXZ = glm::acos(-normXZ.z);
                glm::vec3 normTarget = glm::normalize(targetVec);
                float angleY = glm::acos(glm::dot(normXZ, normTarget));
                glm::vec3 axisXZ = glm::cross(normXZ, normTarget);
                glm::mat4 transform = glm::translate(GameData::positions[e]) * glm::rotate(angleY, axisXZ) * glm::rotate(angleXZ, glm::vec3(0, glm::sign(-normXZ.x), 0));

                //Create entities representing attack (projectiles)
                std::list<Entity> attacks = prefabMap[GameData::pattackmodules[e].attack]();
                    
                float cooldown = 0.0f;
                for (auto i = attacks.begin(); i != attacks.end(); ++i) {
                    Entity attack = *i;
                    if (attack == INVALID_ENTITY) {
                        continue;
                    }
                    //Transform positions and velocity relative to attacker
                    GameData::positions[attack] = transform * glm::vec4(GameData::positions[attack], 1);
                    GameData::velocities[attack] = transform * glm::vec4(GameData::velocities[attack], 0);
                    //Set Hostility
                    GameData::hostilities[attack].team = GameData::hostilities[e].team;
                    GameData::hostilities[attack].hostileTo = GameData::hostilities[e].hostileTo;
                    //Set creator
                    GameData::tags[attack] += ComponentTags::Created;
                    GameData::creators[attack] = e;
                    cooldown = cooldown < GameData::spawnrates[attack] ? GameData::spawnrates[attack] : cooldown;
                }
                //Set cooldown TODO, should be its own component in an attack
                GameData::pattackmodules[e].cooldown = cooldown;
            }
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

void EntityComponentSystem::sysBuild()
{
    for (Entity e = 0; e < MAX_ENTITIES; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }

        //check if this entity has a health component
        if ((GameData::tags[e] & ComponentTags::Builder) == ComponentTags::Builder)
        {

            if (!GameData::retplaces[e].validTarget) {
                
                if (GameData::retplaces[e].reticle != INVALID_ENTITY) {
                    //printf("Deleting reticle entity %d\n", GameData::retplaces[e].reticle);
                    GameData::activity[GameData::retplaces[e].reticle] = false;
                    GameData::retplaces[e].reticle = INVALID_ENTITY;
                }
                
                
                continue; 
            }
            glm::vec3 targetVec = GameData::retplaces[e].targetPos - GameData::positions[e];
            glm::vec3 normXZ = glm::normalize(glm::vec3(targetVec.x, 0, targetVec.z));
            float angleXZ = glm::acos(-normXZ.z);
            glm::vec3 normTarget = glm::normalize(targetVec);
            glm::mat4 transform = glm::translate(GameData::retplaces[e].targetPos) * glm::rotate(angleXZ, glm::vec3(0, glm::sign(-normXZ.x), 0));

            if (GameData::retplaces[e].place) {
                if ( (GameData::retplaces[e].reticle !=INVALID_ENTITY) && (GameData::activity[GameData::retplaces[e].reticle])) {
                    
                    Entity b = prefabMap[GameData::retplaces[e].buildingPrefab]().front();

                    if (b == INVALID_ENTITY) { printf("Too many entities, trying to place building\n"); }
                    else {
                        //Transform positions and velocity relative to attacker
                        GameData::positions[b] = transform * glm::vec4(GameData::positions[b], 1);
                        GameData::velocities[b] = transform * glm::vec4(GameData::velocities[b], 0);
                        //Set creator
                        GameData::tags[b] += ComponentTags::Created;
                        GameData::creators[b] = e;
                    }

                }
                else {
                    //printf("Attempted to place but reticle is invaid\n");
                }
                GameData::retplaces[e].place = false;
            }
            
            
            Entity r;
            if ((GameData::retplaces[e].reticle == INVALID_ENTITY) || (!GameData::activity[GameData::retplaces[e].reticle])) {
                //printf("Generating new reticle\n");
                r = prefabMap[GameData::retplaces[e].reticlePrefab]().front();
            }
            else {
                r = GameData::retplaces[e].reticle;
            }

            if (r == INVALID_ENTITY) { //printf("Too many entities, trying to place reticle\n"); 
            }
            else {
                //Transform positions and velocity relative to attacker
                GameData::positions[r] = glm::vec3(0, 0, 0);
                GameData::positions[r] = transform * glm::vec4(GameData::positions[r], 1);
                GameData::velocities[r] = transform * glm::vec4(GameData::velocities[r], 0);
                GameData::tags[r] ^= ComponentTags::Velocity;
                //Set creator
                GameData::tags[r] += ComponentTags::Created;
                GameData::creators[r] = e;
                GameData::retplaces[e].reticle = r;
            }
        }
    }
}

//Create an empty projectile 
Entity EntityComponentSystem::createEntity(int begin, int end)
{
    //Once we get instancing and modelID rendering setup,
    for (int i = begin; i < end; ++i) {
        if (!GameData::activity[i]) {
            GameData::activity[i] = true;
            return i;
        }
    }
    //No available inactive enemy slots
    return INVALID_ENTITY;
}

glm::vec3 EntityComponentSystem::computeRaycast(glm::vec3& pos, glm::vec3& dir, float tminog, float tmaxog)
{
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
                    //printf("Firing at entity %d\n", e);
                    tfirst = tmin;
                }
            }
        }
    }
    /*
    if (tfirst == 1024) {
        printf("Firing at nothing\n");
    }
    */


    return pos + (dirNorm * tfirst);
}

void EntityComponentSystem::dealDamage(Entity source, Entity target, float damage)
{
    GameData::healths[target].curHealth -= damage;
    if (GameData::tags[source] & ComponentTags::Created) {
        source = GameData::creators[source];
    }
    GameData::combatLogs[GameData::logpos].source = source;
    GameData::combatLogs[GameData::logpos].target = target;
    GameData::combatLogs[GameData::logpos].damage = damage;
    if (GameData::healths[target].curHealth <= 0) {
        causeDeath(source, target);
    }
    GameData::logpos++;
}

void EntityComponentSystem::causeDeath(Entity source, Entity target)
{
    if ((GameData::tags[target] & ComponentTags::Dead) == ComponentTags::Dead) {
        return;
    }
    GameData::tags[target] |= ComponentTags::Dead;

    if (source == target) { return; }
    GameData::combatLogs[GameData::logpos].source = source;
    GameData::combatLogs[GameData::logpos].target = target;
    GameData::combatLogs[GameData::logpos].killed = true;

    GameData::logpos++;
}

bool EntityComponentSystem::colCheck(Entity e, Entity o)
{
    glm::vec3 maxe = GameData::positions[e] + GameData::colliders[e].AABB;
    glm::vec3 maxo = GameData::positions[o] + GameData::colliders[o].AABB;
    glm::vec3 mine = GameData::positions[e] - GameData::colliders[e].AABB;
    glm::vec3 mino = GameData::positions[o] - GameData::colliders[o].AABB;
    return glm::all(glm::lessThan(mine, maxo)) && glm::all(glm::lessThan(mino, maxe));
}

