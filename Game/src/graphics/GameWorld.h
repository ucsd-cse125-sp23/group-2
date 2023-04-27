#pragma once
#include "Player.h"
#include "Mob.h"
#include "Skybox.h"
#include "Shader.h"
#include "../GameConstants.h"
#include "../NetworkData.h"
class GameWorld {

private:
    int currID;
    std::array <Player*, NUM_CLIENTS> players;
    std::array <Mob*, NUM_ENEMIES> mobs;
    Skybox* env;

public:
    void init();

    //
    void update(ServertoClientData& incomingData, int id);

    //render all active entities
    void draw(const glm::mat4& viewProjMtx, Shader* shader, Shader* skyboxShader);
};