#include "ServerGame.h"

ServerGame::ServerGame(void)
{

    // set up the server network to listen
    network = new ServerNetwork();

    initializeGame();

    debug[0] = '\0';

    curTick = 0;

}

//Populate Component Arrays
void ServerGame::initializeGame()
{
    initPlayers();
    initEnemies();
    //initTowers();
    //initResources();
    //initProjectiles();
}

void ServerGame::initPlayers()
{
    //Initialize Players
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        GameData::activity[i] = true;
        GameData::positions[i] = glm::vec3(0, 0, 0);
        GameData::velocities[i] = glm::vec3(0, 0, 0);
        GameData::colliders[i] = { glm::vec3(1, 1, 1) };
        GameData::models[i].modelID = MODEL_ID_ROVER;
        GameData::models[i].asciiRep = 'P';
        GameData::healths[i].maxHealth = GameData::healths[i].curHealth = PLAYER_BASE_HEALTH;
        GameData::hostilities[i].team = Teams::Players;
        GameData::hostilities[i].hostileTo = Teams::Environment+Teams::Martians;
        GameData::attackmodules[i].attack = Prefabs::ProjectileBasic;
        GameData::attackmodules[i].targetPos = glm::vec3(0, 0, 0);
        GameData::attackmodules[i].cooldown = 0;


        GameData::tags[i] =
            ComponentTags::Position +
            ComponentTags::Velocity +
            ComponentTags::Model +
            ComponentTags::Collidable+
            ComponentTags::RigidBody+
            ComponentTags::Health +
            ComponentTags::Hostility +
            ComponentTags::Attacker;
        //TODO: Other Model Data
    }
    //TODO: Change
    //Manually set spawn positions
    GameData::positions[0] = glm::vec3(0, 0, 0);
    GameData::positions[1] = glm::vec3(0, 0, 3);
    GameData::positions[2] = glm::vec3(3, 0, 0);
    GameData::positions[3] = glm::vec3(3, 0, 3);

}

void ServerGame::initEnemies()
{
    
    /*
    //Initialize Enemies
    for (int i = ENEMY_START; i < ENEMY_END; i++)
    {
        GameData::activity[i] = false;
        GameData::positions[i] = glm::vec3(31, 0, 31);
        memcpy(GameData::pathStructs[i].pathNodes, testPath, sizeof(GameData::pathStructs[i].pathNodes));
        GameData::pathStructs[i].currentNode = 0;
        GameData::pathStructs[i].moveSpeed = PLAYER_MVSPD;
        GameData::colliders[i] = { glm::vec3(1, 1, 1) };
        GameData::models[i].asciiRep = 'E';
        //GameData::rigidbodies[i].fixed = true;
        GameData::healths[i].maxHealth = GameData::healths[i].curHealth = ENEMY_BASE_HEALTH;
        GameData::coldmg[i].damage = 30.0f;
        GameData::hostilities[i].team = Teams::Martians;
        GameData::hostilities[i].hostileTo = Teams::Players + Teams::Towers;
        GameData::tags[i] =
            ComponentTags::Active +
            ComponentTags::Position +
            ComponentTags::Velocity +
            ComponentTags::PathData +
            ComponentTags::Model +
            ComponentTags::Collidable+
            ComponentTags::DiesOnCollision +
            ComponentTags::RigidBody +
            ComponentTags::Health +
            ComponentTags::CollisionDmg +
            ComponentTags::Hostility;
    }
    */
}


//TESTING: STAGGERED SPAWN (TODO: REMOVE AFTER TESTING)

int curEntity = ENEMY_START;
void ServerGame::testing_staggeredSpawn()
{
    if (curTick >= ENEMY_SPAWNDELAY_TICKS)
    {
        curEntity = createEnemy();
        if (curEntity != INVALID_ENTITY) {
            //cout << "Entity " << curEntity << " Spawned in!\n";
            GameData::activity[curEntity] = true;
            GameData::pathStructs[curEntity].currentNode = 0;
            GameData::positions[curEntity] = GameData::pathStructs[curEntity].pathNodes[0];
            GameData::healths[curEntity].curHealth = ENEMY_BASE_HEALTH;
            GameData::models[curEntity].asciiRep = 'E';
        }

        curTick = 0;
    }
}

Entity ServerGame::createEnemy()
{
    Entity i = ECS::createEntity();
    if (i == INVALID_ENTITY) {
        return i;
    }
    //Create Path (TEMP FOR TESTING) TODO: REMOVE FOR FINAL VERSION
    glm::vec3 testPath[PATH_LENGTH] = { glm::vec3(15,0,31), glm::vec3(31,0,15), glm::vec3(15,0,15), glm::vec3(0,0,31), glm::vec3(0,0,15), glm::vec3(31,0,7), glm::vec3(31,0,0), glm::vec3(0, 0, 0) };
    GameData::activity[i] = true;
    memcpy(GameData::pathStructs[i].pathNodes, Paths::path1, sizeof(GameData::pathStructs[i].pathNodes));
    GameData::pathStructs[i].currentNode = 0;
    GameData::positions[i] = GameData::pathStructs[i].pathNodes[0];
    GameData::pathStructs[i].moveSpeed = ENEMY_GND_BASE_MVSPD;
    GameData::colliders[i] = { glm::vec3(1, 1, 1) };
    GameData::models[i].asciiRep = 'E';
    //GameData::rigidbodies[i].fixed = true;
    GameData::healths[i].maxHealth = GameData::healths[i].curHealth = ENEMY_BASE_HEALTH;
    GameData::coldmg[i].damage = ENEMEY_GND_BASE_DMG;
    GameData::hostilities[i].team = Teams::Martians;
    GameData::hostilities[i].hostileTo = Teams::Players + Teams::Towers;
    GameData::tags[i] =
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
    return i;
}

//TODO
void ServerGame::initTowers()
{

    //TESTING: Create a towers
    GameData::activity[TOWER_START] = true;
    GameData::positions[TOWER_START] = glm::vec3(1, 0, 15);
    GameData::turrets[TOWER_START].damage = TURRET_BASE_DMG;
    GameData::turrets[TOWER_START].range = 5;
    GameData::models[TOWER_START].asciiRep = 'T';
    GameData::hostilities[TOWER_START].team = Teams::Martians;
    GameData::hostilities[TOWER_START].hostileTo = Teams::Players + Teams::Towers;
    GameData::tags[TOWER_START] =
        ComponentTags::Position +
        ComponentTags::Model +
        ComponentTags::Turret +
        ComponentTags::Hostility;

    for (int i = TOWER_START + 1; i < TOWER_END; i++)
    {
        GameData::activity[i] = false;
    }
}

//TODO
void ServerGame::initResources()
{

}

//TODO
void ServerGame::initProjectiles()
{
    for (int i = PROJECTILE_START; i < PROJECTILE_END; i++)
    {
        GameData::activity[i] = false;
    }
}

// Update function called every tick
void ServerGame::update()
{
    // get new clients
    if (network->acceptNewClient())
    {
        printf("New client has been connected to the server\n");
    }

    //Receve Input
    receiveFromClients();

    handleInputs();

    EntityComponentSystem::update();

    //Print debug message buffer
    printf(debug);
    debug[0] = '\0';

    testing_staggeredSpawn(); //TODO: Remove this after testing concludes

    if (curTick % 4 == 0) {
        //asciiView();
    }
    curTick++;
}



void ServerGame::handleInputs()
{

    char msg[100];
    msg[0] = '\0';

    for(int i = 0; i < NUM_CLIENTS; i++)
    {
        glm::vec3 camDirection;
        glm::vec3 camPosition;
        GameData::attackmodules[i].isAttacking = false;
        while (!incomingDataLists[i].empty())
        {
            ClienttoServerData in = incomingDataLists[i].front();
            GameData::velocities[i] = glm::vec3(0,GameData::velocities[i].y,0);

            if ( ((in.moveLeft ^ in.moveRight)) || ((in.moveForward ^ in.moveBack))) {
                float camAngle = in.camAngleAroundPlayer;
                glm::vec3 moveDirection = glm::rotate(glm::radians(camAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::normalize(glm::vec4(in.moveLeft - in.moveRight, 0.0f, in.moveForward - in.moveBack, 0.0f));
                GameData::models[i].modelOrientation = -camAngle + glm::degrees(glm::acos(moveDirection.y));
                GameData::velocities[i] += PLAYER_MVSPD * moveDirection;
            }

            if (in.shoot) {
                GameData::attackmodules[i].isAttacking = in.shoot;
                camDirection = in.camDirectionVector;
                camPosition = in.camPosition;

            }

            if (in.jump && GameData::positions[i].y <= 0) {
                GameData::velocities[i].y = PLAYER_JPSPD;
            }
            incomingDataLists[i].pop();
        }

        if (GameData::attackmodules[i].isAttacking && GameData::attackmodules[i].cooldown <= 0) {
            playerAttack(i, camDirection, camPosition);
            //printf("ShootingInput\n");
        }
        //in.print(msg);
    }

}


void ServerGame::sendPackets()
{
    //Send Data to Clients
    ServertoClientData newPackage;
    packageData(newPackage);
    network->sendActionPackets(newPackage);
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
    GameData::attackmodules[e].targetPos = ECS::computeRaycast(campos, camdir, glm::distance(campos, GameData::positions[e])+glm::length(GameData::colliders[e].AABB), FLT_MAX);
    //printf("Targer pos (%f, %f, %f)\n", GameData::attackmodules[e].targetPos.x, GameData::attackmodules[e].targetPos.y, GameData::attackmodules[e].targetPos.z);
}
