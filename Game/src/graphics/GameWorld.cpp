#include "GameWorld.h"
float GameWorld::prevX, GameWorld::prevY,GameWorld::currX, GameWorld::currY, GameWorld::scrollY = 0;

void GameWorld::init() {
	currID = 0;
	env = new Skybox();
	env->setSkyShader(new Shader("../shaders/skybox.vert", "../shaders/skybox.frag"));
	env->setEnvShader(new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag"));

	models[MODEL_ID_CUBE] = new ObjectModel("../assets/cube/cube.obj");
	models[MODEL_ID_ROVER] = new ObjectModel("../assets/rover/rover.obj");

	models[MODEL_ID_MOB] = new ObjectModel("../assets/martian/martian.obj");
	models[MODEL_ID_MOB_TANK] = new ObjectModel("../assets/martian/martian_tank.obj");
	models[MODEL_ID_MOB_MINI] = new ObjectModel("../assets/martian/martian_fast.obj");
	models[MODEL_ID_MOB_FLYING] = new ObjectModel("../assets/ufo/ufo_v2.obj");
	models[MODEL_ID_MOB_TRACTOR] = new ObjectModel("../assets/ufo/ufo_v2.obj");

	models[MODEL_ID_TOWER] = new ObjectModel("../assets/tower/Tower.obj");
	models[MODEL_ID_TESLA] = new ObjectModel("../assets/tesla/tower_tesla.obj");
	models[MODEL_ID_RAILGUN] = new ObjectModel("../assets/railgun/tower_railgun.obj");
	models[MODEL_ID_BARRIER] = new ObjectModel("../assets/barricade/barricade_wood.obj");
	models[MODEL_ID_TOWER_INVALID] = new ObjectModel("../assets/tower/Tower_invalid.obj");
	models[MODEL_ID_TESLA_INVALID] = new ObjectModel("../assets/tesla/tower_tesla_invalid.obj");
	models[MODEL_ID_RAILGUN_INVALID] = new ObjectModel("../assets/railgun/tower_railgun_invalid.obj");
	models[MODEL_ID_BARRIER_INVALID] = new ObjectModel("../assets/barricade/barricade_wood.obj");
	models[MODEL_ID_TOWER_L2] = new ObjectModel("../assets/tower/Tower_L2.obj");
	models[MODEL_ID_TESLA_L2] = new ObjectModel("../assets/tesla/tower_tesla_L2.obj");
	models[MODEL_ID_RAILGUN_L2] = new ObjectModel("../assets/railgun/tower_railgun_L2.obj");
	models[MODEL_ID_TOWER_L3] = new ObjectModel("../assets/tower/Tower_L3.obj");
	models[MODEL_ID_TESLA_L3] = new ObjectModel("../assets/tesla/tower_tesla_L3.obj");
	models[MODEL_ID_RAILGUN_L3] = new ObjectModel("../assets/railgun/tower_railgun_L3.obj");
	models[MODEL_ID_TOWER_L4] = new ObjectModel("../assets/tower/Tower_L4.obj");
	models[MODEL_ID_TESLA_L4] = new ObjectModel("../assets/tesla/tower_tesla_L4.obj");
	models[MODEL_ID_RAILGUN_L4] = new ObjectModel("../assets/railgun/tower_railgun_L4.obj");
	models[MODEL_ID_RESOURCE] = new ObjectModel("../assets/tree/tree.obj");
	models[MODEL_ID_RESOURCE_STONE] = new ObjectModel("../assets/crystal_rock/crystal_rock.obj");

	models[MODEL_ID_PROJECTILE_ROVER] = new ObjectModel("../assets/projectiles/projectile_rover.obj");
	models[MODEL_ID_PROJECTILE_MARTIAN] = new ObjectModel("../assets/projectiles/projectile_martian.obj");
	models[MODEL_ID_PROJECTILE_SPREAD] = new ObjectModel("../assets/projectiles/projectile_spreadshot.obj");
	models[MODEL_ID_PROJECTILE_RAPID] = new ObjectModel("../assets/projectiles/projectile_rapidfire.obj");


	models[MODEL_ID_BASE] = new ObjectModel("../assets/base/base.obj");
	models[MODEL_ID_BEAR] = new ObjectModel("../assets/bear/bear.obj");
	models[MODEL_ID_BEAR_HEAD] = new ObjectModel("../assets/bear/head.obj");
	models[MODEL_ID_BEAR_LARM] = new ObjectModel("../assets/bear/larm.obj");
	models[MODEL_ID_BEAR_RARM] = new ObjectModel("../assets/bear/rarm.obj");
	models[MODEL_ID_BEAR_LLEG] = new ObjectModel("../assets/bear/lleg.obj");
	models[MODEL_ID_BEAR_RLEG] = new ObjectModel("../assets/bear/rleg.obj");
	models[MODEL_ID_BEAR_BODY] = new ObjectModel("../assets/bear/body.obj");
	models[MODEL_ID_PATH_STRAIGHT] = new ObjectModel("../assets/paths/path_straight.obj");
	models[MODEL_ID_PATH_CORNER] = new ObjectModel("../assets/paths/path_corner.obj");

	models[MODEL_ID_POWERUP_RAPID] = new ObjectModel("../assets/powerups/rapid_fire.obj"); //TODO: add proper models
	models[MODEL_ID_POWERUP_SPREAD] = new ObjectModel("../assets/powerups/spreadshot.obj");


	//models[MODEL_ID_SUNGOD] = new ObjectModel("../assets/crystal_rock/crystal_rock.obj");

	shaders[MODEL_ID_CUBE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_ROVER] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_MOB] = new Shader("../shaders/martian_shader.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_MOB_TANK] = new Shader("../shaders/martian_shader.vert", "../shaders/model_loading.frag"); //dunno if we want new shading for the tank and minis
	shaders[MODEL_ID_MOB_MINI] = new Shader("../shaders/martian_shader.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_MOB_FLYING] = new Shader("../shaders/ufo_shader.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_MOB_TRACTOR] = new Shader("../shaders/ufo_shader.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_TOWER] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_RAILGUN] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TESLA] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BARRIER] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TOWER_L2] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_RAILGUN_L2] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TESLA_L2] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");	
	shaders[MODEL_ID_TOWER_L3] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	
	shaders[MODEL_ID_RAILGUN_L3] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TESLA_L3] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");	
	shaders[MODEL_ID_TOWER_L4] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_RAILGUN_L4] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TESLA_L4] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TOWER_INVALID] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_RAILGUN_INVALID] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TESLA_INVALID] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BARRIER_INVALID] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_RESOURCE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_RESOURCE_STONE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_PROJECTILE_ROVER] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_PROJECTILE_MARTIAN] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_PROJECTILE_SPREAD] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_PROJECTILE_RAPID] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_BASE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR_HEAD] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR_LARM] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR_RARM] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR_LLEG] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR_RLEG] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR_BODY] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_PATH_STRAIGHT] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_PATH_CORNER] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	//shaders[MODEL_ID_SUNGOD] = new ObjectModel("../assets/crystal_rock/crystal_rock.obj");

	shaders[MODEL_ID_POWERUP_SPREAD] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_POWERUP_RAPID] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	healthShader = new Shader("../shaders/shader.vert", "../shaders/shader.frag");
	ObjectModel* healthModel = new ObjectModel("../assets/cube/cube.obj");
	playerHealth = 1.0f;
	for (int i = 0; i < MAX_ENTITIES; i++) {
		entities[i] = new RenderEntity(i);
		AABBs[i] = new Cube(shaders[MODEL_ID_CUBE], models[MODEL_ID_CUBE]);
		healths[i] = new HealthBar(healthShader, healthModel);
	}
	cam = new Camera();
	shakeScreen = false;
	screenShakeOn = false;
	startTime = 0;
	effect = new EffectSystem();
}

void GameWorld::update(ServertoClientData& incomingData, int id) {

	if (entities[id]->getPosition() != incomingData.positions[id]) {
		effect->playerJumpEffect(entities[id]->getPosition());
	}
	for (int i = 0; i < incomingData.activity.size(); i++) {

		if (incomingData.activity[i] && incomingData.models[i].renderCollider) {
				AABBs[i]->setActive(true);
				AABBs[i]->update(incomingData.positions[i], incomingData.colliders[i].AABB);
		}
		else {
			AABBs[i]->setActive(false);
		}
		//if active and should render health bar
		if (incomingData.activity[i]
			&& i != id
			&& incomingData.healths[i].curHealth / incomingData.healths[i].maxHealth < 1) {
			healths[i]->setActive(true);
			healths[i]->update(cam->getCameraPosition(), incomingData.positions[i] +  glm::vec3(0.0f,incomingData.colliders[i].AABB.y, 0.0f), 
				 incomingData.healths[i].curHealth, incomingData.healths[i].maxHealth);
		}
		else {
			healths[i]->setActive(false);
		}
		if (incomingData.activity[i] && incomingData.models[i].modelID != MODEL_ID_NO_MODEL) {
			entities[i]->setActive(true);
			entities[i]->setModel(models[incomingData.models[i].modelID]);
			entities[i]->setModelID(incomingData.models[i].modelID);
			entities[i]->setShader(shaders[incomingData.models[i].modelID]);
			entities[i]->update(incomingData.positions[i], incomingData.models[i].modelOrientation, incomingData.models[i].scale);
		}
		else {
			entities[i]->setActive(false);
		}
	}

	//camera stuff
	int maxDelta = 100;
	int dx = glm::clamp((int)(currX - prevX), -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)(currY - prevY)), -maxDelta, maxDelta);
	prevX = (int)currX;
	prevY = (int)currY;

	//screen shake
	float currHealth = incomingData.healths[id].curHealth / incomingData.healths[id].maxHealth;
	if (currHealth < playerHealth) {
		shakeScreen = true;
		playerHealth = currHealth;
	}
	else {
		shakeScreen = false;
	}

	float currTime = glfwGetTime();

	if (shakeScreen) {
		screenShakeOn = true;
		startTime = glfwGetTime();
	}

	if (currTime > (startTime + 0.3f)) {
		screenShakeOn = false;
		shakeScreen = false;
	}
	cam->update(entities[id]->getPosition(), dx, dy, scrollY, screenShakeOn, currTime);


	//effects
	if (currWaveTimer != incomingData.waveTimer) {
		for (int i = 0; i < incomingData.clogsize; i++) {
				newCLogs.push(incomingData.combatLogs[i]);
		}

		for (int i = 0; i < incomingData.slogsize; i++) {
			if (!incomingData.soundLogs[i].stop) {
				newSLogs.push(incomingData.soundLogs[i]);
			}
		}
		currWaveTimer = incomingData.waveTimer;
	}
	while (!newCLogs.empty()) {
		if (newCLogs.top().killed) {
			std::cout << "KILLED\n";
			int target = newCLogs.top().target;
			effect->resourceEffect(entities[target]->getPosition(), entities[target]->getModelID());
		}
		newCLogs.pop();
	}
	while (!newSLogs.empty()) {
		int source = newSLogs.top().source;
		int sound = newSLogs.top().sound;
		if (sound == SOUND_ID_JUMP) {
			std::cout << "JUMPED\n";
			effect->playerJumpEffect(entities[source]->getPosition());
		}
		else if (sound == SOUND_ID_ATTACK && !newSLogs.top().stop) {
			if (entities[source]->getModelID() == MODEL_ID_TESLA) {
				effect->teslaAttackEffect(entities[source]->getPosition());
			}
		}
		newSLogs.pop();
	}
	effect->update(currTime);
}

//render all active entities
void GameWorld::draw() {
	float currTime = float(glfwGetTime());
	const glm::mat4& viewProjMtx = cam->GetViewProjectMtx();
	env->draw(viewProjMtx);
	for (RenderEntity* e : entities) {

		if (e->getActive()) {
			e->draw(viewProjMtx, currTime, cam);
		}
	}

	effect->draw(viewProjMtx, cam);
	for (HealthBar* h : healths) {
		if (h->getActive()) {
			h->draw(viewProjMtx);
		}
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (Cube* c : AABBs) {
		if (c->getActive()) {
			c->draw(viewProjMtx);
		}
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void GameWorld::cursor_callback(GLFWwindow* window, double cX, double cY) {
	currX = cX;
	currY = cY;
}
