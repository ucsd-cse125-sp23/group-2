#include "ServerGame.h"

ServerGame::ServerGame(void)
{
    std::srand(std::time(nullptr));
    currentStatus = init;
    // set up the server network to listen
    network = new ServerNetwork();

    initializeGame();

    debug[0] = '\0';

    curTick = 0;
    clientsConnected = 0;
}

//Populate Component Arrays
void ServerGame::initializeGame()
{
    initPlayers();
    initPrefabs();
    initBase();
    initResources();
    initWaves();
}

void ServerGame::initPlayers()
{
    prefabMap[Prefab::Players]();
    for (int i = 0; i < NUM_RESOURCE_TYPES; ++i) {
        GameData::playerdata.resources[i] = 1000;
    }

}

void ServerGame::initWaves()
{
    WaveData::currentWave = START_WAVE;
    WaveData::waveTick = ENEMY_SPAWNDELAY_TICKS;

    enemy e;
    //Intro wave (32 basic enemies), random paths
    for (int i = 0; i < 32; i++)
    {
        e = { Prefabs::EnemyGroundBasic, rand() % (Paths::pathCount), 1.0f };
        WaveData::waves[0].push(e);
    }
    
    //Wave 2 (Introduce basic flying)
    //Randomly placed flying enemies
    for (int i = 0; i < 32; i++)
    {
        e = { Prefabs::EnemyFlyingBasic, rand() % (Paths::pathCount), 1.0f };
        WaveData::waves[1].push(e);
    }
    WaveData::waves[1].back().cooldown = 10.0f;
    //eight basics on each path
    for (int i = 0; i < 8; i++)
    {
        for (int p = 0; p < Paths::pathCount; p++)
        {
            e = { Prefabs::EnemyGroundBasic, p, 0 };
            WaveData::waves[1].push(e);
        }
        WaveData::waves[1].back().cooldown = 1.0f;
    }
    
    //Wave 3 (Introduce mini and tank)
    //four mini, four flying on each path
    for (int i = 0; i < 8; i++)
    {
        for (int p = 0; p < Paths::pathCount; p++)
        {
            e = { Prefabs::EnemyGroundMini, p, 0 };
            WaveData::waves[2].push(e);
        }
        WaveData::waves[2].back().cooldown = 2.0f;
        for (int p = 0; p < Paths::pathCount; p++)
        {
            e = { Prefabs::EnemyFlyingBasic, p, 0 };
            WaveData::waves[2].push(e);
        }
        WaveData::waves[2].back().cooldown = 2.0f;
    }
    WaveData::waves[2].back().cooldown = 15.0f;
    //Randomly placed tanks
    for (int i = 0; i < 16; i++)
    {
        e = { Prefabs::EnemyGroundTank, rand() % (Paths::pathCount), 1.0f };
        WaveData::waves[2].push(e);
    }
    //Wave 4 (Introduce abductors)
    //Eight basics on each path
    for (int i = 0; i < 16; i++)
    {
        for (int p = 0; p < Paths::pathCount; p++)
        {
            e = { Prefabs::EnemyGroundBasic, p, 0 };
            WaveData::waves[3].push(e);
        }
        WaveData::waves[3].back().cooldown = 1.0f;
    }
    //Randomly placed tractors
    for (int i = 0; i < 4; i++)
    {
        e = { Prefabs::EnemyFlyingTractor, rand() % (Paths::pathCount), 1.0f };
        WaveData::waves[3].push(e);
    }
    //Eight fliers + Eight more basics on each path
    for (int i = 0; i < 16; i++)
    {
        for (int p = 0; p < Paths::pathCount; p++)
        {
            e = { Prefabs::EnemyFlyingBasic, p, 0 };
            WaveData::waves[3].push(e);
        }
        WaveData::waves[3].back().cooldown = 1.0f;
        for (int p = 0; p < Paths::pathCount; p++)
        {
            e = { Prefabs::EnemyGroundBasic, p, 0 };
            WaveData::waves[3].push(e);
        }
        WaveData::waves[3].back().cooldown = 1.0f;
    }
    //Wave 5 (Random Wave)
    //60 entirely random enemies
    for (int i = 0; i < 60; i++)
    {
        e = {WaveData::enemyTypes[rand()%NUM_ENEMY_TYPES], rand() % (Paths::pathCount), 1.0f};
        WaveData::waves[4].push(e);
    }
    //Wave 6 (Challenge Wave)
    //Minis and fliers down mid, tractors and basics on sides, tanks on flank
    for (int i = 0; i < 8; i++)
    {
        for (int p = 0; p < Paths::pathCount; p++)
        {
            if (p == 2) {
                e = { Prefabs::EnemyGroundMini, p, 0 };
            }
            if (p == 0 || p == 4) {
                e = { Prefabs::EnemyGroundTank, p, 0 };
            }
            if (p == 1 || p == 3) {
                e = { Prefabs::EnemyGroundBasic, p, 0 };
            }
            WaveData::waves[5].push(e);
        }
        WaveData::waves[5].back().cooldown = 1.0f;
        for (int p = 0; p < Paths::pathCount; p++)
        {
            if (p == 2) {
                e = { Prefabs::EnemyFlyingBasic, p, 0 };
            }
            if (p == 1 || p == 3) {
                e = { Prefabs::EnemyFlyingTractor, p, 0 };
            }
            if (p == 0 || p == 4) {
                continue;
            }
            WaveData::waves[5].push(e);
        }
        WaveData::waves[5].back().cooldown = 1.0f;
    }
    WaveData::waves[5].back().cooldown = 30.0f;
    //MINIs EVERYWHERE
    for (int i = 0; i < 16; i++)
    {
        for (int p = 0; p < Paths::pathCount; p++)
        {
            e = { Prefabs::EnemyGroundMini, p, 0 };
            WaveData::waves[5].push(e);
        }
        WaveData::waves[5].back().cooldown = 2.0f;
    }
    //Buncha tanks now
    for (int i = 0; i < 16; i++)
    {
        for (int p = 0; p < Paths::pathCount; p++)
        {
            e = { Prefabs::EnemyGroundTank, p, 0 };
            WaveData::waves[5].push(e);
        }
        WaveData::waves[5].back().cooldown = 2.0f;
    }
    //Tractors and fliers
    for (int i = 0; i < 16; i++)
    {
        for (int p = 0; p < Paths::pathCount; p++)
        {
            e = { Prefabs::EnemyFlyingBasic, p, 0 };
            WaveData::waves[5].push(e);
        }
        WaveData::waves[5].back().cooldown = 2.0f;
        for (int p = 0; p < Paths::pathCount; p++)
        {
            enemy e = { Prefabs::EnemyFlyingTractor, p, 0 };
            WaveData::waves[5].push(e);
        }
        WaveData::waves[5].back().cooldown = 2.0f;
    }

    //Boss wave
    enemy boss = { Prefabs::EnemyBoss, Paths::bossPath, 10.0f};
    WaveData::waves[WAVE_COUNT - 1].push(boss);
    //Suplement with a little bit of everything
    //Minis and fliers on the outskirts, tanks and basics on the inside. Tractors everywhere
    for (int i = 0; i < 8; i++)
    {
        for (int p = 0; p < Paths::pathCount; p++)
        {
            if (p == 0 || p == 4) {
                e = { Prefabs::EnemyFlyingBasic, p, 0.0f };
            }
            if (p == 1 || p == 3) {
                e = { Prefabs::EnemyGroundBasic, p, 0.0f };
            }
            if (p == 2) {
                continue;
            }
            WaveData::waves[WAVE_COUNT - 1].push(e);
        }
        WaveData::waves[WAVE_COUNT - 1].back().cooldown = 2.0f;
        for (int p = 0; p < Paths::pathCount; p++)
        {
            if (p == 0 || p == 4) {
                e = { Prefabs::EnemyGroundMini, p, 0.0f };
            }
            if (p == 1 || p == 3) {
                e = { Prefabs::EnemyGroundTank, p, 0.0f };
            }
            if (p == 2) {
                continue;
            }
            WaveData::waves[WAVE_COUNT - 1].push(e);
        }
        WaveData::waves[WAVE_COUNT - 1].back().cooldown = 2.0f;
        for (int p = 0; p < Paths::pathCount; p++)
        {
            if (p != Paths::bossPath) {
                enemy e = { Prefabs::EnemyFlyingTractor, p, 0.0f };
                WaveData::waves[WAVE_COUNT - 1].push(e);
            }
        }
        WaveData::waves[WAVE_COUNT - 1].back().cooldown = 1.0f;
    }
}

void ServerGame::initBase()
{
    home = prefabMap[Prefabs::Home]().front();
}

void ServerGame::waveSpawner()
{
    static float spawnCooldown = 0;

    
    if (WaveData::waveTick <= 0)
    {
        WaveData::currentWave++;
        if (WaveData::currentWave < WAVE_COUNT)
        {
            WaveData::waveTick = WaveData::waveTimers[WaveData::currentWave];
        }
    }

    if (WaveData::currentWave >= 0 && WaveData::currentWave < WAVE_COUNT)
    {
        if (!WaveData::waves[WaveData::currentWave].empty())
        {
            while (spawnCooldown <= 0.0f)
            {
                enemy e = WaveData::waves[WaveData::currentWave].front();
                WaveData::waves[WaveData::currentWave].pop();
                list<Entity> l = prefabMap[e.id]();
                if (l.front() != INVALID_ENTITY)
                {
                    GameData::pathStructs[l.front()].path = e.path;
                    GameData::positions[l.front()] = Paths::path[e.path][0];
                    Collision::updateColTable(l.front());
                    if (!WaveData::waves[WaveData::currentWave].empty())
                    {
                        spawnCooldown = e.cooldown;
                    }
                    else {
                        break;
                    }
                }
            }
            spawnCooldown-= 1.0f/TICK_RATE;
        }
    }

    WaveData::waveTick--;
    
}

//Spawn Initial assortment of resources
void ServerGame::initResources()
{
    std::vector<glm::vec3> positions = PoissonDisk::genPoints();

    printf("Number of resources %d", positions.size());

    std::vector<Entity> resources = std::vector<Entity>();

    for (glm::vec3 pos : positions) {
        Entity e;
        if(std::rand() > RAND_MAX/2)
            e = prefabMap[Prefabs::BASIC_STONE_RESOURCE]().front();
        else
            e = prefabMap[Prefabs::BASIC_WOOD_RESOURCE]().front();
        if (e != INVALID_ENTITY) {
            GameData::positions[e] = pos - glm::vec3(WORLD_X/2, 0, WORLD_Z/2);
            GameData::tags[e] |= ComponentTags::DiesOnCollision;
            GameData::colliders[e].colwith |= CollisionLayer::UIObj + CollisionLayer::StaticObj;
            Collision::updateColTable(e);
            resources.push_back(e);
        }
        //printf("Pos is %f, %f, %f\n", pos.x, pos.y, pos.z);
    }
    for (Entity e : resources) {
        float scaleFactor = (rand() % 32) / ((float)32) + 0.5;
        GameData::colliders[e].AABB.x *= scaleFactor;
        GameData::colliders[e].AABB.y *= scaleFactor;
        GameData::colliders[e].AABB.z *= scaleFactor;
        for (Entity p : Paths::pathlist) {
            ECS::colCheck(e, p);
        }
        for (Entity b : Boundry::boundlist) {
            ECS::colCheck(e, b);
        }
        ECS::colCheck(e, MAX_ENTITIES_NOBASE);
        ECS::resolveCollisions();
        GameData::tags[e] ^= ComponentTags::DiesOnCollision;
        GameData::colliders[e].colwith ^= CollisionLayer::UIObj + CollisionLayer::StaticObj;
        GameData::models[e].modelOrientation = rand() % 360;
        GameData::models[e].scale = scaleFactor;
    }
    //Clear the sound and combat logs to remove data from the resource spawning
    GameData::clogpos = 0;
    GameData::slogpos = 0;

}

// Update function called every tick
void ServerGame::update()
{
    // get new clients
    if (network->acceptNewClient())
    {
        clientsConnected++;
        printf("Client %u connected\n", clientsConnected);
    }
    //Receve Input
    receiveFromClients();
    //auto startTime = std::chrono::steady_clock::now();
    //auto endTime = std::chrono::steady_clock::now();
    //auto duration = std::chrono::duration<double, std::milli>(endTime - startTime);
    switch (currentStatus){
    case init:
        if (clientsConnected >= NUM_PLAYERS) {
            currentStatus = game;
            printf("All players connected, starting main update loop!\n");
        }
        break;
    case game:
        GameData::slogpos = 0;

        handleInputs();
        //startTime = std::chrono::steady_clock::now();
        EntityComponentSystem::update();
        //endTime = std::chrono::steady_clock::now();
        //duration = std::chrono::duration<double, std::milli>(endTime - startTime);
        //std::cout << "ECS took " << duration.count() << " milliseconds.\n";

        waveSpawner();

        checkStatus();

        if (curTick % 4 == 0) {
            //asciiView();
        }
        curTick++;
        break;
    case loss:
        break;
    case win:
        break;
    default:
        printf("Invalid server state!\n");
    }




    //Print debug message buffer
    printf(debug);
    debug[0] = '\0';


}

const int NUM_PLAYER_ATTACK = 3;
const Prefab playerReticleArray[NUM_TOWER_PREFAB] = { Prefabs::TowerReticleBasic, Prefabs::TowerReticleRailgun, Prefabs::TowerReticleTesla, Prefabs::TowerReticleBarrier };
const Prefab playerBuildingArray[NUM_TOWER_PREFAB] = { Prefabs::TowerBasic, Prefabs::TowerRailgun, Prefabs::TowerTesla, Prefabs::TowerBarrier };


void ServerGame::handleInputs()
{

    char msg[100];
    msg[0] = '\0';

    for(int i = 0; i < NUM_CLIENTS; i++)
    {
        //Decrement cooldown
        GameData::playerdata.actioncooldown[i]--;
        glm::vec3 camDirection;
        glm::vec3 camPosition;
        bool target = false;
        bool jump = false;
        Entity choose = INVALID_ENTITY;
        while (!incomingDataLists[i].empty())
        {
            if ((GameData::tags[i] & ComponentTags::Dead) == ComponentTags::Dead) {
                std::queue<ClienttoServerData> empty;
                std::swap(incomingDataLists[i], empty);
                continue;
            }
            ClienttoServerData in = incomingDataLists[i].front();
            GameData::velocities[i].velocity = glm::vec3(0, GameData::velocities[i].velocity.y, 0);
            camDirection = in.camDirectionVector;
            camPosition = in.camPosition;
            if (((in.moveLeft ^ in.moveRight)) || ((in.moveForward ^ in.moveBack))) {
                float camAngle = in.camAngleAroundPlayer;
                glm::vec3 moveDirection = glm::normalize(glm::rotate(glm::radians(camAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::normalize(glm::vec4(in.moveLeft - in.moveRight, 0.0f, in.moveForward - in.moveBack, 0.0f)));
                GameData::models[i].modelOrientation = -camAngle + glm::degrees(glm::acos(moveDirection.y));
                GameData::velocities[i].velocity += PLAYER_MVSPD * moveDirection;
            }

            if (in.shoot) {
                target = in.shoot;
            }

            if (in.build) {
                changeState(i, PlayerState::Build);
                if (in.selected > NUM_TOWER_TYPES) {
                    in.selected = NUM_TOWER_TYPES - 1;
                }
                if (in.selected < 0) {
                    in.selected = 0; //BAD BAD CODE
                }
                if (GameData::retplaces[i].reticlePrefab != playerReticleArray[in.selected]) {
                    if (GameData::retplaces[i].reticle != INVALID_ENTITY) {
                        ECS::causeDeath(GameData::retplaces[i].reticle, GameData::retplaces[i].reticle);
                    }
                }
                GameData::retplaces[i].buildingPrefab = playerBuildingArray[in.selected];
                GameData::retplaces[i].reticlePrefab = playerReticleArray[in.selected];
                choose = INVALID_ENTITY;
            }
            else if (in.upgrade) {
                changeState(i, PlayerState::Upgrading);
                choose = playerUpgrade(i, camDirection, camPosition, TOWER_PLACEMENT_RANGE);
                if (GameData::retplaces[i].reticle != INVALID_ENTITY) {
                    ECS::causeDeath(GameData::retplaces[i].reticle, GameData::retplaces[i].reticle);
                    GameData::retplaces[i].reticle = INVALID_ENTITY;
                }
            }
            else {
                changeState(i, PlayerState::Default); //May be slow

                if (GameData::retplaces[i].reticle != INVALID_ENTITY) {
                    ECS::causeDeath(GameData::retplaces[i].reticle, GameData::retplaces[i].reticle);
                    GameData::retplaces[i].reticle = INVALID_ENTITY;
                }
                choose = INVALID_ENTITY;

            }

            if (in.jump){
                jump = true;
            }
            incomingDataLists[i].pop();
        }

        


        if (GameData::states[i] == PlayerState::Build) {
            //printf("Calling Player build\n");
            playerBuild(i, camDirection, camPosition);
        }
        else if (GameData::states[i] == PlayerState::Upgrading) {
            if (choose != INVALID_ENTITY) {
                GameData::models[choose].upgradeSelected = true;
            }
        }

        if (target) {
            if (GameData::states[i] == PlayerState::Build) {
                GameData::retplaces[i].place = true;
            }
            else if (GameData::states[i] == PlayerState::Upgrading) {
                if (GameData::playerdata.actioncooldown[i] < 0) {
                    if (choose != INVALID_ENTITY) {
                        ECS::applyUpgrade(i, choose);
                        GameData::playerdata.actioncooldown[i] = ACTION_COOLDOWN;
                    }
                }
            }
            else {
                changeState(i, PlayerState::Attack);
                playerAttack(i, camDirection, camPosition);
            }
            //printf("ShootingInput\n");
        }
        else if(GameData::states[i] == PlayerState::Attack){
            changeState(i, PlayerState::Default);
        }

        if (jump && GameData::rigidbodies[i].grounded) {
            GameData::velocities[i].velocity.y = PLAYER_JPSPD;
            // Add jumping sound to sound log
            EntityComponentSystem::logSound(i, SOUND_ID_JUMP);
        }
        //in.print(msg);
    }

}


void ServerGame::sendPackets()
{
    if (currentStatus == game) {
        //Send Data to Clients
        packageData(gameState);
        network->sendActionPackets(gameState);
    }
}

void ServerGame::initPrefabs()
{
    Paths::pathlist = prefabMap[Prefabs::PathColliders]();
    Boundry::boundlist = prefabMap[Prefabs::Bounds]();
    /*
    for (Entity e : pathlist) {
        if (e != INVALID_ENTITY) {
            printf("Collider for ent %d, is (%f, %f, %f)\n", e, GameData::colliders[e].AABB.x, GameData::colliders[e].AABB.y, GameData::colliders[e].AABB.z);
            printf("Position for ent %d, is (%f, %f, %f)\n", e, GameData::positions[e].x, GameData::positions[e].y, GameData::positions[e].z);
            if (ComponentTags::Collidable & GameData::tags[e]) {
                printf("can Collide\n");
            }
            if (GameData::activity[e]) {
                printf("is active\n");
            }
        }
    }
    */
    //Init Path Objects

}

void ServerGame::receiveFromClients()
{

    network->receiveDeserialize(incomingDataLists);

}

void ServerGame::packageData(ServertoClientData& data)
{
    data.positions = GameData::positions;
    data.models = GameData::models;
    data.activity = GameData::activity;
    data.healths = GameData::healths;
    data.combatLogs = GameData::combatLogs;
    data.clogsize = GameData::clogpos;
    data.soundLogs = GameData::soundLogs;
    data.slogsize = GameData::slogpos;
    data.currentWave = WaveData::currentWave + 1;
    data.numWaves = WAVE_COUNT;
    data.playerData = GameData::playerdata;
    data.buildcosts = buildcosts;
    data.serverStatus = currentStatus;
    data.colliders = GameData::colliders;
    data.waveTimer = WaveData::waveTick / TICK_RATE;
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        data.playerData.playerStates[i] = GameData::states[i];
    }
}

const int GRID_X = 32;
const int GRID_Z = 32;
void ServerGame::asciiView() {
    // "Clear" the screen
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
    // Create "grid"
    char grid[GRID_X][GRID_Z];
    // Initialize it with all periods
    memset(grid, '.', sizeof(grid));
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        // Continue to next entity if this one is not active
        if (!GameData::activity[e]) { continue; }
        // Continue to next entity if this one doesn't have position
        if ((GameData::tags[e] & ComponentTags::Position) != ComponentTags::Position) { continue; }
        // Continue to next entity if this one doesn't have model
        if ((GameData::tags[e] & ComponentTags::Model) != ComponentTags::Model) { continue; }
        // Floor entity position
        int xCoord = floor(GameData::positions[e].x);
        int zCoord = floor(GameData::positions[e].z);
        // If its within grid bounds, add to grid
        if (xCoord >= 0 && xCoord < GRID_X && zCoord >= 0 && zCoord < GRID_Z) {
            grid[xCoord][zCoord] = GameData::models[e].asciiRep;
        }
    }
    // Print the grid
    for (int i = 0; i < GRID_X; ++i) {
        for (int j = 0; j < GRID_Z; ++j) {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

void ServerGame::playerAttack(Entity e, glm::vec3& camdir, glm::vec3& campos)
{
    GameData::pattackmodules[e].targetPos = ECS::computeRaycast(campos, camdir, glm::distance(campos, GameData::positions[e])+glm::length(GameData::colliders[e].AABB), FLT_MAX);
    //printf("Targer pos (%f, %f, %f)\n", GameData::pattackmodules[e].targetPos.x, GameData::pattackmodules[e].targetPos.y, GameData::pattackmodules[e].targetPos.z);
}

void ServerGame::changeState(Entity e, State post)
{
    GameData::tags[e] ^= GameData::states[e];
    GameData::states[e] = post;
    GameData::tags[e] |= post;
}

void ServerGame::playerBuild(Entity i, glm::vec3& camdir, glm::vec3& campos)
{
    GameData::retplaces[i].renderRet = true;
    GameData::retplaces[i].validTarget = true;
    if (camdir.y >= 0) {
        //printf("You're looking up\n");
        GameData::retplaces[i].validTarget = false;
        GameData::retplaces[i].renderRet = false;
        return;
    }
    glm::vec3 dirYNorm = camdir / (camdir.y*-1);
    glm::vec3 targetpos = glm::vec3(campos.x + dirYNorm.x * campos.y, 0, campos.z + dirYNorm.z * campos.y);
    float angle = 0;
    if(GameData::retplaces[i].reticlePrefab == Prefabs::TowerReticleBarrier){
        Entity p = ECS::findClosestPathCollider(targetpos);
        if (p != INVALID_ENTITY && (glm::distance(targetpos, GameData::positions[p]) <= SNAP_RANGE)) {
            targetpos = GameData::positions[p];
            angle = GameData::models[p].modelOrientation;
        }
        else {
            GameData::retplaces[i].validTarget = false;
        }
    }

    if (glm::distance(targetpos, GameData::positions[i]) > TOWER_PLACEMENT_RANGE) {
        //printf("Out of range\n");

        GameData::retplaces[i].validTarget = false;
        GameData::retplaces[i].renderRet = false;
        return;
    }

    GameData::retplaces[i].targetPos = targetpos;
    GameData::retplaces[i].targetOrientation = angle;
    //printf("Valid Target\n");
}

void ServerGame::checkStatus() {
    if (WaveData::currentWave == WAVE_COUNT && WaveData::waveTick <= 0) {
        printf("WIN! :D\n");
        currentStatus = win;
    }
    if (!GameData::activity[home]) {
        printf("LOSE! :(\n");
        currentStatus = loss;
    }
}

Entity ServerGame::playerUpgrade(Entity e, glm::vec3& camdir, glm::vec3& campos, float range)
{
    Entity target = INVALID_ENTITY;
    ECS::computeRaycast(campos, camdir, glm::distance(campos, GameData::positions[e]) + glm::length(GameData::colliders[e].AABB), FLT_MAX, &target);
    if (target == INVALID_ENTITY) {
        return target;
    }
    if (GameData::tags[target] & ComponentTags::Upgradeable) {
        return target;
    }
    return INVALID_ENTITY;
}
