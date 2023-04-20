#include "EntityComponentSystem.h"

namespace GameData 
{
  std::array<Tag, MAX_ENTITIES> tags;

  std::array<Active, MAX_ENTITIES> activity;
  std::array<Position, MAX_ENTITIES> positions;
  std::array<Velocity, MAX_ENTITIES> velocities;
  std::array<PathData, MAX_ENTITIES> pathStructs;
  std::array<Model, MAX_ENTITIES> models;
}

//Call all systems each update
void EntityComponentSystem::update()
{
    //sysPathing();
    sysMovement();
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

            //Check if entity has reached its currently tracked destination
            if (GameData::positions[e] == GameData::pathStructs[e].pathNodes[GameData::pathStructs[e].currentNode]) 
            {
                //Node reached, increment current Node
                printf("Node Reached by Test Entity\n");
                GameData::pathStructs[e].currentNode++;
            }

            //Check if entity has reached the final pathNode (currentNode now out of bounds)
            if (GameData::pathStructs[e].currentNode >= PATH_LENGTH) 
            {
                //TODO: (Temporarily Set Enemey to inactive once it reaches the end of the path)
                GameData::activity[e] = false;
                printf("Test entity reached the end of its path\n");
                continue;
            }

            //Update entity velocity vector to move towards tracked node
            glm::vec3 direction = GameData::pathStructs[e].pathNodes[GameData::pathStructs[e].currentNode] - GameData::positions[e];
            float magnitude = sqrt(pow(direction.x, 2) + pow(direction.y, 2) + pow(direction.z, 2));
            GameData::velocities[e] = (direction / magnitude) * GameData::pathStructs[e].moveSpeed;
        }
    }
}
