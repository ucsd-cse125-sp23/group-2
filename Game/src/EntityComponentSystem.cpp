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

void EntityComponentSystem::update()
{
    sysMovement();
}

void EntityComponentSystem::sysMovement()
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        
    }
}
