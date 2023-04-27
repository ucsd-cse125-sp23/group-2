#include "EntityComponentSystem.h"

namespace GameData 
{
  std::array<Tag, MAX_ENTITIES> tags;

  std::array<Active, MAX_ENTITIES> activity;
  std::array<Position, MAX_ENTITIES> positions;
  std::array<Velocity, MAX_ENTITIES> velocities;
  std::array<PathData, MAX_ENTITIES> pathStructs;
  std::array<Model, MAX_ENTITIES> models;
  std::array<Collider, MAX_ENTITIES> colliders;
  std::queue<CollisionEvent> colevents;
  std::array<RigidBodyInfo, MAX_ENTITIES> rigidbodies;
  std::array<Health, MAX_ENTITIES> healths;
}

//Call all systems each update
void EntityComponentSystem::update()
{
    sysPathing();
    sysMovement();
    sysDetectCollisions();
    resolveCollisions();
    dmgAll();
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
        if ((GameData::tags[e] & (ComponentTags::RigidBody)) == ComponentTags::RigidBody)
        {
            if (!GameData::rigidbodies[e].fixed) {
                GameData::positions[e] += ce.pen;
            }

        }




        if ((GameData::tags[e] & (ComponentTags::DiesOnCollision)) == ComponentTags::DiesOnCollision) {
            GameData::activity[e] = false;
        }
    }
}

void EntityComponentSystem::dmgAll()
{
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        if (GameData::activity[i] == false) {
            continue;
        }
        if ((ComponentTags::Health & GameData::tags[i]) == ComponentTags::Health) {
            GameData::healths[i].curHealth -= 0.1f;
            if (GameData::healths[i].curHealth <= 0.0) {
                GameData::activity[i] = false;
            }
            printf("Entity %d, health: %f\n", i, GameData::healths[i].curHealth);
        }
    }
}
