#include "EntityComponentSystem.h"

namespace GameData
{
  std::array<Tag, MAX_ENTITIES> tags;
  std::array<Active, MAX_ENTITIES> activity;
  std::array<Position, MAX_ENTITIES> positions;
  std::array<VelocityData, MAX_ENTITIES> velocities;
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
  std::array<HitscanAttackModule, MAX_ENTITIES> hattackmodules;
  std::array<CombatLog, CLOG_MAXSIZE> combatLogs;
  std::array<SoundLog, SLOG_MAXSIZE> soundLogs;
  std::array<Creator, MAX_ENTITIES> creators;
  std::array<SpawnRate, MAX_ENTITIES> spawnrates;
  int clogpos = 0;
  int slogpos = 0;
  std::array<State, MAX_ENTITIES> states;
  std::array<ReticlePlacement, MAX_ENTITIES> retplaces;
  std::array<HomingData, MAX_ENTITIES> homingStructs;
  std::array<AbductionData, MAX_ENTITIES> abductionStructs;
  std::array<ResourceContainer, MAX_ENTITIES> resources;
  std::array<Points, MAX_ENTITIES> pointvalues;
  AllPlayerData playerdata;
  std::array<AOEAttackModule, MAX_ENTITIES> AOEattackmodules;
  std::array<Upgradeable, MAX_ENTITIES> upgradedata;
}

//Call all systems each update
void EntityComponentSystem::update()
{
    GameData::clogpos = 0;
    sysEnemyAI();
    sysDeathStatus();
    sysAttacks();
    sysPathing();
    sysHoming();
    sysAbduction();
    sysMovement();
    sysGravity();
    sysTurret();
    //auto startTime = std::chrono::steady_clock::now();
    sysDetectCollisions();
    //auto endTime = std::chrono::steady_clock::now();
    //auto duration = std::chrono::duration<double, std::milli>(endTime - startTime);
    //std::cout << "Detect took " << duration.count() << " milliseconds.\n";

    //startTime = std::chrono::steady_clock::now();
    resolveCollisions();
    //endTime = std::chrono::steady_clock::now();
    //duration = std::chrono::duration<double, std::milli>(endTime - startTime);
    //::cout << "Resolve took " << duration.count() << " milliseconds.\n";
    sysBuild();
    sysLifeSpan();
}

namespace Collision {
    std::unordered_set<Entity> cgrid[gridx][gridz];
    void updateColTable(Entity e)
    {
        int numerase = Collision::cgrid[GameData::colliders[e].xpos][GameData::colliders[e].zpos].erase(e);
        if (numerase != 1) {
            //("New Element being added to table/ Collision tabl\n");
        }
        int xpos = (int)((GameData::positions[e].x + WORLD_X / 2) / side);
        int zpos = (int)((GameData::positions[e].z + WORLD_Z / 2) / side);
        if (xpos < 0 || xpos >= gridx || zpos < 0 || zpos >= gridz) {
            //printf("Entity %d has left bounds (will not collide)\n", e);
            return;
        }
        Collision::cgrid[xpos][zpos].insert(e);
        GameData::colliders[e].xpos = xpos;
        GameData::colliders[e].zpos = zpos;
    }
}

void EntityComponentSystem::sysEnemyAI()
{
    for (Entity e = 0; e < MAX_ENTITIES; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }

        //check if this entity has the stalker AI tag
        if ((GameData::tags[e] & ComponentTags::Stalker) == ComponentTags::Stalker)
        {
            switch (GameData::states[e])
            {
            case enemyState::Pathing:
                //check if players nearby
                for (int p = 0; p < NUM_PLAYERS; p++)
                {
                    if (glm::distance(GameData::positions[p], GameData::positions[e]) <= AGRO_RANGE)
                    {
                        changeState(e, enemyState::Homing);
                        GameData::homingStructs[e].trackedEntity = p;
                    }
                }
                break;
            case enemyState::Homing:
                //If hostile to tracked entity
                if (!(GameData::hostilities[e].hostileTo & GameData::hostilities[GameData::homingStructs[e].trackedEntity].team)) { continue; }
                //check for DEAGRO or death
                if (glm::distance(GameData::positions[e], GameData::positions[GameData::homingStructs[e].trackedEntity]) >= DEAGRO_RANGE || (GameData::tags[GameData::homingStructs[e].trackedEntity] & ComponentTags::Dead) == ComponentTags::Dead)
                {
                    changeState(e, enemyState::Pathing);
                    rePath(e);
                }
                break;
            default:
                printf("Entity %u is a hunter with invalid state!\n", e);
                changeState(e, enemyState::Pathing);
                rePath(e);
            }
        }
        else if ((GameData::tags[e] & ComponentTags::Hunter) == ComponentTags::Hunter)
        {
            //Find closest enemy to shoot
            Entity closestEnemy = e; //initialized to turret ID in case of no valid target found
            float closestDistance = ATTACK_RANGE + 1; //Set closest found distance to out of range

            //Loop Thru enemies and find one in range
            for (Entity i = 0; i < MAX_ENTITIES; i++)
            {
                //Check if enemy is active
                if (!GameData::activity[i]) { continue; }
                //Check if hostileto
                if (!(GameData::hostilities[e].hostileTo & GameData::hostilities[i].team)) { continue; }
                //Check if enemy is in range
                float enemyDistance = glm::distance(GameData::positions[e], GameData::positions[i]);
                //check if this enemy is the new closest entity
                if (enemyDistance < closestDistance)
                {
                    closestEnemy = i;
                    closestDistance = enemyDistance;
                }
            }
            switch (GameData::states[e])
            {
            case enemyState::Pathing:
                //If a valid target was found, fire at them
                if (closestEnemy != e)
                {
                    GameData::hattackmodules[e].target = closestEnemy;
                    GameData::pattackmodules[e].targetPos = GameData::positions[closestEnemy] + GameData::velocities[closestEnemy].velocity;
                    changeState(e, enemyState::ShootingProjectile);
                }
                break;
            case enemyState::ShootingProjectile:
                GameData::hattackmodules[e].target = closestEnemy;
                GameData::pattackmodules[e].targetPos = GameData::positions[closestEnemy] + GameData::velocities[closestEnemy].velocity;
                if (closestEnemy == e)
                {
                    changeState(e, enemyState::Pathing);
                    rePath(e);
                }
                break;
            default:
                printf("Entity %u is a trapper with invalid state!\n", e);
                changeState(e, enemyState::Pathing);
                rePath(e);
            }
        }
        else if ((GameData::tags[e] & ComponentTags::Trapper) == ComponentTags::Trapper)
        {
            switch (GameData::states[e])
            {
            case enemyState::Homing:
                if (GameData::abductionStructs[e].captive != INVALID_ENTITY)
                {
                    printf("Now pathing\n");
                    changeState(e, enemyState::Pathing);
                    rePath(e);
                    GameData::rigidbodies[e].fixed = true; //BAD BAD CODE, but stops the weird movement
                }

                //find nearest player
                for (int p = 0; p < NUM_PLAYERS; p++)
                {
                    if (glm::distance(GameData::positions[p], GameData::positions[e]) <= glm::distance(GameData::positions[GameData::homingStructs[e].trackedEntity], GameData::positions[e]))
                    {
                        GameData::homingStructs[e].trackedEntity = p;
                    }
                }
            case enemyState::Pathing:
                if (GameData::abductionStructs[e].captive == INVALID_ENTITY)
                {
                    changeState(e, enemyState::Homing);
                }
                break;
            default:
                printf("Entity %u is a trapper with invalid state!\n", e);
                changeState(e, enemyState::Pathing);
                rePath(e);
            }
        }
    }
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
            GameData::positions[e] = GameData::positions[e] + GameData::velocities[e].velocity;
            
            //Update Collision Table
            if ((GameData::tags[e] & ComponentTags::Collidable) == ComponentTags::Collidable) {
                Collision::updateColTable(e);
            }
        }
        if (e < NUM_PLAYERS) {
            //Keep Entity within World dimensionsc if player
            if (GameData::positions[e].x > WORLD_X / 2) {
                GameData::positions[e].x = WORLD_X / 2;
            }
            if (GameData::positions[e].z > WORLD_Z / 2) {
                GameData::positions[e].z = WORLD_Z / 2;

            }
            if (GameData::positions[e].x < -1.0 * WORLD_X / 2.0) {
                GameData::positions[e].x = -1.0 * WORLD_X / 2.0;
            }
            if (GameData::positions[e].z < -1.0 * WORLD_Z / 2.0) {
                GameData::positions[e].z = -1.0 * WORLD_Z / 2.0;
            }
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
            if (GameData::positions[e].y > (GROUND_HEIGHT+GameData::colliders[e].AABB.y)) {
                GameData::velocities[e].velocity.y += GRAVITY;
                GameData::rigidbodies[e].grounded = false;
            }
            else {
                GameData::positions[e].y = (GROUND_HEIGHT+GameData::colliders[e].AABB.y);
                if (GameData::velocities[e].velocity.y < 0) {
                    // Add landing sound to sound log
                    logSound(e, SOUND_ID_LAND);
                    GameData::velocities[e].velocity.y = 0;
                }
                GameData::rigidbodies[e].grounded = true;
            }
        }
    }
}

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
            glm::vec3 curPos = GameData::positions[e];
            curPos = glm::vec3(curPos.x, 0, curPos.z);
            bool closeEnough = glm::distance(nodePos, curPos) < 1;
            if (closeEnough)
            {
                //Node reached, increment current Node
                //std::cout << "Entity " << e << " Reached Node " << GameData::pathStructs[e].currentNode << "\n";
                GameData::pathStructs[e].currentNode++;
            }

            //Check if entity has reached the final pathNode (currentNode now out of bounds)
            if (GameData::pathStructs[e].currentNode >= PATH_LENGTH)
            {
                causeDeath(e, e);
                //std::cout << "Entity " << e << " Reached the end of its path! (Despawning...)\n";
                continue;
            }

            //Update entity velocity vector to move towards tracked node
            glm::vec3 direction = Paths::path[GameData::pathStructs[e].path][GameData::pathStructs[e].currentNode] - GameData::positions[e];
            //Modify height of direction vector before normalization if applicable
            if (GameData::velocities[e].flying)
            {
                direction = glm::vec3(direction.x, direction.y + FLYING_HEIGHT, direction.z);
            }
            else {
                direction.y = 0;
            }
            GameData::velocities[e].velocity = glm::normalize(direction) * GameData::velocities[e].moveSpeed;
        }
    }
}

void EntityComponentSystem::sysHoming()
{
    for (Entity e = 0; e < MAX_ENTITIES; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }
        //check if this entity has a homing component
        if ((GameData::tags[e] & ComponentTags::HomingData) == ComponentTags::HomingData)
        {
            Entity target = GameData::homingStructs[e].trackedEntity;
            if (target != INVALID_ENTITY)
            {
                //Update entity velocity vector to move towards tracked entity
                glm::vec3 trackedPos = GameData::positions[target];
                glm::vec3 direction = trackedPos - GameData::positions[e];
                //Modify height of direction vector before normalization if applicable
                if (GameData::velocities[e].flying)
                {
                    direction = glm::vec3(direction.x, direction.y + FLYING_HEIGHT, direction.z);
                }
                else {
                    //direction.y = 0; Maybe TODO????
                }
                direction += GameData::homingStructs[e].offset; //This means only somethng with offset can track in y direction

                //printf("Dirrection is (%f, %f, %f)\n", direction.x, direction.y, direction.z);
                GameData::velocities[e].velocity = glm::normalize(direction) * GameData::velocities[e].moveSpeed;
            }
        }
    }
}

void EntityComponentSystem::sysAbduction() {
    for (Entity e = 0; e < MAX_ENTITIES; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }

        //check if this entity has a abductor component
        if ((GameData::tags[e] & ComponentTags::Abductor) == ComponentTags::Abductor)
        {
            Entity c = GameData::abductionStructs[e].captive; //Entity being held captive
            Entity t = GameData::homingStructs[e].trackedEntity; //Entity being homed on
            //check if there already is a captive
            if (c == INVALID_ENTITY)
            {
                //if entity being homed on is in abduction range, and can be abducted
                if (glm::distance(GameData::positions[t], GameData::positions[e]) < ABDUCT_RANGE && !(GameData::tags[t] & (ComponentTags::Abducted | ComponentTags::Dead)))
                {
                    logSound(e, SOUND_ID_ATTACK);
                    //start abducting
                    GameData::abductionStructs[e].abductionTimeLeft -= 1.0f / TICK_RATE;
                    if (GameData::abductionStructs[e].abductionTimeLeft <= 0) {
                        GameData::abductionStructs[e].captive = GameData::homingStructs[e].trackedEntity;
                        GameData::tags[GameData::homingStructs[e].trackedEntity] |= ComponentTags::Abducted;
                    }
                }
                else
                {
                    GameData::abductionStructs[e].abductionTimeLeft = ABDUCT_TIMER;
                }
            }
            else
            {
                //If either entity is dead, destroy abductor captive relationship
                if ((GameData::tags[c] & ComponentTags::Dead) == ComponentTags::Dead || (GameData::tags[e] & ComponentTags::Dead) == ComponentTags::Dead) {
                    GameData::abductionStructs[e].captive = INVALID_ENTITY;
                    GameData::tags[c] ^= ComponentTags::Abducted;
                }
                else {
                    glm::vec3 direction = (GameData::positions[e] - glm::vec3(0.0f, GameData::colliders[e].AABB.y, 0.0f) - GameData::positions[c]);
                    GameData::velocities[c].velocity = glm::normalize(direction) * GameData::velocities[c].moveSpeed * 0.01f + GameData::velocities[e].velocity;
                    //Stop captive from shooting when fully abducted
                    GameData::pattackmodules[c].cooldown = 1.0f;
                    GameData::rigidbodies[e].grounded = false;
                }
            }

        }
    }
}

void EntityComponentSystem::sysTurret()
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

            //std::list<Entity> inRange = getTargetsInRange(GameData::positions[e], GameData::turrets[e].range, GameData::hostilities[e].hostileTo);


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
                GameData::hattackmodules[e].target = closestEnemy;
                GameData::pattackmodules[e].targetPos = GameData::positions[closestEnemy] + GameData::velocities[closestEnemy].velocity;
                changeState(e, GameData::turrets[e].attackState);

                //Set model orientation
                glm::vec3 projDir = glm::normalize(glm::vec3(GameData::models[e].dirNorm.x, 0, GameData::models[e].dirNorm.z));
                GameData::models[e].modelOrientation = (projDir.z < 0) ? -glm::degrees(glm::acos(projDir.x)) : glm::degrees(glm::acos(projDir.x));
            }
            else 
            {
               changeState(e, towerStates::Idle);
            }
        }
    }
}

void EntityComponentSystem::sysDetectCollisions()
{
    std::unordered_set<Entity> neighbors = std::unordered_set<Entity>();
    for (Entity e = 0; e < MAX_ENTITIES_NOBASE; e++)
    {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }
        GameData::coldmg[e].cooldown -= 1.0f / TICK_RATE;
        Tag collides = ComponentTags::Position + ComponentTags::Collidable;
        //check if this entity can can collide
        if ((GameData::tags[e] & collides) == collides)
        {

            int xpos = GameData::colliders[e].xpos;
            int zpos = GameData::colliders[e].zpos;
            neighbors.clear();
            
            
            //Construct all neigbors
            for (int j = glm::max(zpos - 1, 0); j <= glm::min(zpos + 1, (int)Collision::gridz - 1); ++j) {
                for (int i = glm::max(xpos - 1, 0); i <= glm::min(xpos + 1, (int)Collision::gridx - 1); ++i) {
                    neighbors.insert(Collision::cgrid[i][j].begin(), Collision::cgrid[i][j].end());
                }
            }
            
            


            //Check collisions with everything else
            for (Entity o : neighbors)
            {
                colCheck(e, o);
                
            }

            //Check base and path collision seperatley
            for (Entity p : Paths::pathlist) {
                colCheck(e, p);
            }
            colCheck(e, MAX_ENTITIES_NOBASE);
            colCheck(MAX_ENTITIES_NOBASE, e);
        }


    }
}

Entity EntityComponentSystem::findClosestPathCollider(glm::vec3 origin)
{


    //find closest path node
    float closestDistance = 100;
    Entity closestPath = INVALID_ENTITY;
    //loop thru all pathNodes to find the closest one
    for (Entity p : Paths::pathlist) {
        float distance = glm::distance(origin, GameData::positions[p]);
        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestPath = p;
        }
    }
    return closestPath;


}

bool EntityComponentSystem::applyUpgrade(Entity play, Entity target)
{
    if ((GameData::tags[target] & ComponentTags::Upgradeable)==ComponentTags::Upgradeable) {
        bool hasenough = true;
        for (int i = 0; i < NUM_RESOURCE_TYPES; ++i) {
            hasenough &= GameData::upgradedata[target].cost[i] <= GameData::playerdata.resources[i];
        }
        if (!hasenough) {
            return false;
        }
        Entity up = prefabMap[GameData::upgradedata[target].upgrade]().front();
        if (up == INVALID_ENTITY) {
            return false;
        }
        causeDeath(target, target);


        //Subtract resources
        for (int i = 0; i < NUM_RESOURCE_TYPES; ++i) {
            GameData::playerdata.resources[i] -= GameData::upgradedata[target].cost[i];
        }

        //Set position of new tower
        GameData::positions[up] = GameData::positions[target];

        //Add to collision table
        Collision::updateColTable(up);

        return true;

    }
    return false;
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
                if (GameData::rigidbodies[o].fixed) {
                    GameData::positions[e] += ce.pen;
                }
                else {
                    GameData::positions[e] += ce.pen/2.0f;
                }
                if (glm::abs(ce.pen.x) < glm::abs(ce.pen.y) && glm::abs(ce.pen.z) < glm::abs(ce.pen.y)) {
                    GameData::rigidbodies[e].grounded = true;
                    GameData::velocities[e].velocity.y = 0;
                }
            }

        }

        //Check if dies on Collision
        if ((GameData::tags[e] & (ComponentTags::DiesOnCollision)) == ComponentTags::DiesOnCollision) {
            causeDeath(e, e);
            GameData::tags[e] ^= ComponentTags::Collidable;
        }

        //Do on collision damage
        if ((GameData::tags[e] & (ComponentTags::CollisionDmg)) == ComponentTags::CollisionDmg) {
            //Check if hostileto
            if ((GameData::hostilities[e].hostileTo & GameData::hostilities[o].team)) {
                if ((GameData::tags[o] & (ComponentTags::Health)) == ComponentTags::Health && GameData::coldmg[e].cooldown <= 0) {
                    GameData::coldmg[e].cooldown = GameData::coldmg[e].damageRate;
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
            switch (GameData::hostilities[e].team) {
            case Teams::Players:
                if (GameData::playerdata.spawntimers[e] < -1.0f) {
                    GameData::playerdata.spawntimers[e] = RESPAWN_TIMER;
                    GameData::rigidbodies[e].fixed = true;
                }
                else if (GameData::playerdata.spawntimers[e] <= 0) {
                    GameData::playerdata.spawntimers[e] = -2;
                    GameData::positions[e] = PlayerSpawns::spawnpoint[e];
                    GameData::healths[e].curHealth = GameData::healths[e].maxHealth;
                    GameData::tags[e] ^= ComponentTags::Dead;
                    GameData::rigidbodies[e].fixed = false;;
                    Collision::updateColTable(e);
                }
                else {
                    GameData::playerdata.spawntimers[e] = GameData::playerdata.spawntimers[e] - 1.0f / TICK_RATE;
                }
                break;
            case Teams::Martians:
                if (rand() / ((float)RAND_MAX) <= POWERUP_CHANCE) {
                    Entity p = prefabMap[PowerupRandom]().front();
                    if (p != INVALID_ENTITY) {
                        GameData::positions[p] = GameData::positions[e];
                    }
                }
            default:
                GameData::models[e].asciiRep = 'X';
                GameData::activity[e] = false;
                break;
            }
        }

    }
}

void EntityComponentSystem::sysAttacks()
{

    for (int e = 0; e < MAX_ENTITIES; ++e) {
        //Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }
        GameData::pattackmodules[e].cooldown -= 1.0f / TICK_RATE;
        GameData::hattackmodules[e].cooldown -= 1.0f / TICK_RATE;
        GameData::AOEattackmodules[e].cooldown -= 1.0f / TICK_RATE;

        if ((GameData::tags[e] & ComponentTags::AttackerProjectile) == ComponentTags::AttackerProjectile) {
            //printf("Attacker %u is attacking, checking cooldown\n", e);
            if (GameData::pattackmodules[e].cooldown <= 0) {
                //printf("Attacker %u is attacking, creating projectile %d\n", e, GameData::pattackmodules[e].cooldown);
                //printf("Current Pos: %f, %f, %f | Target Pos: %f, %f, %f\n", GameData::positions[e].x, GameData::positions[e].y, GameData::positions[e].z, GameData::pattackmodules[e].targetPos.x, GameData::pattackmodules[e].targetPos.y, GameData::pattackmodules[e].targetPos.z);
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
                    GameData::velocities[attack].velocity = transform * glm::vec4(GameData::velocities[attack].velocity, 0);
                    //Set Hostility
                    GameData::hostilities[attack].hostileTo = GameData::hostilities[e].hostileTo;
                    //Set creator
                    GameData::tags[attack] |= ComponentTags::Created;
                    GameData::creators[attack] = e;
                    cooldown = cooldown < GameData::spawnrates[attack] ? GameData::spawnrates[attack] : cooldown;
                    Collision::updateColTable(attack);
                }
                GameData::pattackmodules[e].cooldown = cooldown;

                // Add attack sound to sound log
                logSound(e, SOUND_ID_ATTACK);
            }
        }

        if ((GameData::tags[e] & ComponentTags::AttackerHitscan) == ComponentTags::AttackerHitscan) {
            if (GameData::hattackmodules[e].cooldown <= 0) {
                GameData::hattackmodules[e].cooldown = GameData::hattackmodules[e].fireRate;
                dealDamage(e, GameData::hattackmodules[e].target, (GameData::hattackmodules[e].damage));
                // Add attack sound to sound log
                logSound(e, SOUND_ID_ATTACK);
            }
        }

        if ((GameData::tags[e] & ComponentTags::AttackerAOE) == ComponentTags::AttackerAOE) {
            if (GameData::AOEattackmodules[e].cooldown <= 0) {
                
                GameData::AOEattackmodules[e].source = GameData::positions[e];
                std::list<Entity> targets = getTargetsInRange(GameData::AOEattackmodules[e].source, GameData::AOEattackmodules[e].range, GameData::hostilities[e].hostileTo);
                //printf("Attempting to fire\n");

                if (!targets.empty()) {
                    GameData::AOEattackmodules[e].cooldown = GameData::AOEattackmodules[e].fireRate; 
                    printf("Non empty targets\n");
                    // Add attack sound to sound log
                    logSound(e, SOUND_ID_ATTACK);
                    for (Entity t : targets) {
                        dealDamage(e, t, (GameData::AOEattackmodules[e].damage));
                    }
                }
                else {
                    // Stop the attack sound
                    logSound(e, SOUND_ID_ATTACK, true);
                }
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
                causeDeath(e, e);
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

        //check if this entity has a build component
        if ((GameData::tags[e] & ComponentTags::Builder) == ComponentTags::Builder)
        {

            if (!GameData::retplaces[e].validTarget) {

                if (GameData::retplaces[e].reticle != INVALID_ENTITY) {
                    //printf("Deleting reticle entity %d\n", GameData::retplaces[e].reticle);
                    causeDeath(GameData::retplaces[e].reticle, GameData::retplaces[e].reticle);
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
                if ( (GameData::retplaces[e].reticle !=INVALID_ENTITY) && (GameData::activity[GameData::retplaces[e].reticle]) && ((GameData::tags[GameData::retplaces[e].reticle] & ComponentTags::Dead) != ComponentTags::Dead)) {
                    //Check build costs
                    bool hasenough = true;
                    for (int i = 0; i < NUM_RESOURCE_TYPES; ++i) {
                        hasenough &= buildcosts[GameData::retplaces[e].buildingPrefab][i] <= GameData::playerdata.resources[i];
                    }

                    if (!hasenough) {
                        //printf("Not enough resoruces\n");
                    }
                    else {
                        Entity b = prefabMap[GameData::retplaces[e].buildingPrefab]().front();

                        if (b == INVALID_ENTITY) { //printf("Too many entities, trying to place building\n");
                        }
                        else {
                            //Transform positions and velocity relative to attacker
                            GameData::positions[b] = transform * glm::vec4(GameData::positions[b], 1);
                            GameData::velocities[b].velocity = transform * glm::vec4(GameData::velocities[b].velocity, 0);
                            //Set creator
                            GameData::tags[b] |= ComponentTags::Created;
                            GameData::creators[b] = e;
                            //Add to score
                            GameData::playerdata.scores[e].towersBuilt++;
                            //Subtract resources
                            for (int i = 0; i < NUM_RESOURCE_TYPES; ++i) {
                                GameData::playerdata.resources[i] -= buildcosts[GameData::retplaces[e].buildingPrefab][i];
                            }
                            //Transform colider
                            if (GameData::retplaces[e].targetOrientation != 0 || GameData::retplaces[e].targetOrientation != 180) {
                                GameData::models[b].modelOrientation = GameData::retplaces[e].targetOrientation;
                                float temp = GameData::colliders[b].AABB.x;
                                GameData::colliders[b].AABB.x = GameData::colliders[b].AABB.z;
                                GameData::colliders[b].AABB.z = temp;
                            }
                            // Add sound to sound log
                            logSound(e, SOUND_ID_BUILD);
                            Collision::updateColTable(b);
                        }
                    }

                }
                else {
                    //printf("Attempted to place but reticle is invaid\n");
                }
                GameData::retplaces[e].place = false;
            }


            Entity r;
            if ((GameData::retplaces[e].reticle == INVALID_ENTITY) || (!GameData::activity[GameData::retplaces[e].reticle]) || ((GameData::tags[GameData::retplaces[e].reticle] & ComponentTags::Dead) == ComponentTags::Dead)) {

                //printf("Generating new reticle, old was %d.\n", GameData::retplaces[e].reticle);

                /*
                if ((GameData::retplaces[e].reticle != INVALID_ENTITY)) {
                    if ((!GameData::activity[GameData::retplaces[e].reticle])) {
                        printf("Not active\n");
                    }
                    if ((((GameData::tags[GameData::retplaces[e].reticle] & ComponentTags::Dead) == ComponentTags::Dead))) {
                        printf("Dead\n");
                    }
                }
                */
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
                GameData::velocities[r].velocity = transform * glm::vec4(GameData::velocities[r].velocity, 0);
                GameData::tags[r] ^= ComponentTags::Velocity;
                if (GameData::retplaces[e].targetOrientation != 0) {
                    GameData::models[r].modelOrientation = GameData::retplaces[e].targetOrientation;
                }
                //Set creator
                GameData::tags[r] |= ComponentTags::Created;
                GameData::creators[r] = e;
                GameData::retplaces[e].reticle = r;
                GameData::positions[r].y = GameData::colliders[r].AABB.y + GROUND_HEIGHT;

                Collision::updateColTable(r);
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
            GameData::states[i] = 0;
            GameData::tags[i] = 0;
            GameData::hostilities[i].team = 0;
            GameData::hostilities[i].hostileTo = 0;
            return i;
        }
    }
    //No available inactive enemy slots
    return INVALID_ENTITY;
}

glm::vec3 EntityComponentSystem::computeRaycast(glm::vec3& pos, glm::vec3& dir, float tminog, float tmaxog, Entity * out)
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
                    if (out) {
                        *out = e;
                    }
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
    if ((GameData::tags[target] & ComponentTags::Dead) == ComponentTags::Dead) {
        return;
    }
    GameData::healths[target].curHealth -= damage;
    if (GameData::tags[source] & ComponentTags::Created) {
        source = GameData::creators[source];
    }
    printf("Ent %d dealing %f dmg to %d\n", source, damage, target);
    if (GameData::clogpos < CLOG_MAXSIZE) {
        // Add damage to combat log
        GameData::combatLogs[GameData::clogpos].source = source;
        GameData::combatLogs[GameData::clogpos].target = target;
        GameData::combatLogs[GameData::clogpos].damage = damage;
        GameData::combatLogs[GameData::clogpos].killed = false;
        GameData::clogpos++;
    } 
    else {
        printf("Exceeded combat logsize server side\n");
    }
    // Add damage sound to sound log
    logSound(target, SOUND_ID_DAMAGE);
    if (GameData::healths[target].curHealth <= 0) {
        causeDeath(source, target);
    }
}

void EntityComponentSystem::causeDeath(Entity source, Entity target)
{

    //Remove  from collision grid
    if ((GameData::tags[target] & ComponentTags::Collidable) == ComponentTags::Collidable) {
        Collision::cgrid[GameData::colliders[target].xpos][GameData::colliders[target].zpos].erase(target);
    }

    if ((GameData::tags[target] & ComponentTags::Dead) == ComponentTags::Dead) {
        return;
    }
    //printf("Ent %d is dead\n", target);


    GameData::tags[target] |= ComponentTags::Dead;

    GameData::velocities[target].velocity = glm::vec3(0);

    
    if (source == target) { return; }
    if (GameData::clogpos < CLOG_MAXSIZE) {
        // Add death to combat log
        GameData::combatLogs[GameData::clogpos].source = source;
        GameData::combatLogs[GameData::clogpos].target = target;
        GameData::combatLogs[GameData::clogpos].killed = true;
        GameData::clogpos++;
    }
    else{
        printf("exceeded combatlog size\n");
    }
    // Add death sound to sound log
    logSound(target, SOUND_ID_DEATH);

    if (source < NUM_PLAYERS) {
        if ((GameData::tags[target] & ComponentTags::WorthPoints) == ComponentTags::WorthPoints) {
            GameData::playerdata.scores[source].points += GameData::pointvalues[target];
            //printf("Adding points\n");
        }
        if ((GameData::tags[target] & ComponentTags::ResourceContainer) == ComponentTags::ResourceContainer) {
            for (int i = 0; i < NUM_RESOURCE_TYPES; ++i) {
                GameData::playerdata.scores[source].resourcesGathered[i] += GameData::resources[target].resources[i];
                GameData::playerdata.resources[i] += GameData::resources[target].resources[i];
                //printf("Gaining Resources %d\n", GameData::playerdata.resources[i]);
            }
        }
        if ((GameData::tags[target] & ComponentTags::Hostility) == ComponentTags::Hostility) {
            if (GameData::hostilities[target].team == Teams::Martians) {
                GameData::playerdata.scores[source].enemiesKilled++;
                //printf("Enemy killed %d\n", GameData::playerdata.scores[source].enemiesKilled);
            }
        }
    }
}

bool EntityComponentSystem::colCheck(Entity e, Entity o)
{
    //Continue to next entity if this one is not active
    if (!GameData::activity[o] || !GameData::activity[e]) { return false; }



    //Continue to next entity if this one is itself
    if (o == e) { return false; }

    Tag collides = ComponentTags::Collidable + ComponentTags::Position;

    if (((GameData::tags[o] & collides) != collides) || ((GameData::tags[e] & collides) != collides)) {
        return false;
    }
   
    //If not on same  collision layer skip
    if (!(GameData::colliders[e].colwith & GameData::colliders[o].colteam)) { return false; }

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
            index = 1;
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
            pen[index - 3] = -1 * diff2[index - 3];
        }
        //printf("Creating collision between %d and %d\n", e, o);
        //Create Collision Event objects in e
        GameData::colevents.push(CollisionEvent{ e, o, pen });
        
        return true;
        //printf("Diff1: %f, %f, %f,  Diff2: %f, %f, %f\n Index: %d, Minn: %f\n", diff1.x, diff1.y, diff1.z, diff2.x, diff2.y, diff2.z, index, min);
    }
    return false;
}

void EntityComponentSystem::rePath(Entity e)
{
    //init values (defaulted to path 0, node 0)
    float closestDistance = glm::distance(GameData::positions[e], Paths::path[0][0]);
    GameData::pathStructs[e].currentNode = 0;
    GameData::pathStructs[e].path = 0;

    //loop thru all pathNodes to find the closest one
    for (int p = 0; p < Paths::pathCount; p++)
    {
        for (int n = 0; n < PATH_LENGTH; n++)
        {
            float distance = glm::distance(GameData::positions[e], Paths::path[p][n]);
            if (distance < closestDistance)
            {
                closestDistance = distance;
                GameData::pathStructs[e].path = p;
                GameData::pathStructs[e].currentNode = n;
            }
        }
    }
}

void EntityComponentSystem::changeState(Entity e, State post)
{
    GameData::tags[e] ^= GameData::states[e];
    GameData::states[e] = post;
    GameData::tags[e] |= post;
}

void EntityComponentSystem::logSound(Entity source, int sound_id, bool stop) {
    if (GameData::slogpos < SLOG_MAXSIZE) {
        GameData::soundLogs[GameData::slogpos].source = source;
        GameData::soundLogs[GameData::slogpos].sound = sound_id;
        GameData::soundLogs[GameData::slogpos].stop = stop;
        GameData::slogpos++;
    }
}

std::list<Entity> EntityComponentSystem::getTargetsInRange(glm::vec3 & source, float & range, TeamID & hostileTo)
{
    int dist = range / Collision::side + 1;
    std::list<Entity> output;
    int xpos = (int)((source.x + WORLD_X / 2) / Collision::side);
    int zpos = (int)((source.z + WORLD_Z / 2) / Collision::side);

    for (int j = glm::max(zpos - dist, 0); j <= glm::min(zpos + dist, (int)Collision::gridz - dist); ++j) {
        for (int i = glm::max(xpos - dist, 0); i <= glm::min(xpos + dist, (int)Collision::gridx - dist); ++i) {
            for (Entity e : Collision::cgrid[i][j]) {
                if (GameData::activity[e] && !(ComponentTags::Dead & GameData::tags[e])) {
                    if (hostileTo & GameData::hostilities[e].team) {
                        output.push_back(e);
                    }
                }
            }
        }
    }
    return output;
}
