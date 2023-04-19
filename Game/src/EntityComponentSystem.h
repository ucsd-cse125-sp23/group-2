#pragma once
#include <vector>
#include <array>
#include "graphics/core.h"
#include "GameConstants.h"

#define PATH_LENGTH 4

//Entity type
using Entity = uint32_t;

//Define Component Tpyes (Max Components = 32)
using Active = bool; //Is Entity active in the scene?
using Position = glm::vec3; //Entity Position in 3D Space
using Velocity = glm::vec3; //Entity Velocity in 3D Space

struct PathData //Data for entity pathing
{
    glm::vec3 pathNodes[PATH_LENGTH]; //3D positions in which entity will need to change direction
    int currentNode; //Index of current node that entity is pathing towards
};

struct Model //3D Model to render for the entity
{
    int modelID;
    //TODO: Other Model Data
};

//Define Component Tags
using Tag = uint32_t;
namespace ComponentTags
{
    constexpr Tag Active    = 0x1;
    constexpr Tag Position  = 0x1 << 1;
    constexpr Tag Velocity  = 0x1 << 2;
    constexpr Tag PathData  = 0x1 << 3;
    constexpr Tag Model     = 0x1 << 4;
}

namespace GameData
{
    //List of entities index by entity ID
    //Entity Tag is a 32 bit int that denotes the components attached to the enitity
    std::array<Tag, MAX_ENTITIES> tags; 

    std::array<Active, MAX_ENTITIES> activity;
    std::array<Position, MAX_ENTITIES> positions;
    std::array<Velocity, MAX_ENTITIES> velocities;
    std::array<PathData, MAX_ENTITIES> pathStructs;
    std::array<Model, MAX_ENTITIES> models;
}

namespace EntityComponentSystem
{

    //Contains Calls to ALL systems
    void update();

    //If Entity has Path Data do pathing, else check velocity comp for players
    void sysMovement();

};