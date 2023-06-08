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
	models[MODEL_ID_MOB_MINI] = new ObjectModel("../assets/martian/martian_fast.obj"); //TODO: make smol
	models[MODEL_ID_MOB_FLYING] = new ObjectModel("../assets/ufo/ufo_v2.obj");
	models[MODEL_ID_MOB_TRACTOR] = new ObjectModel("../assets/ufo/ufo_v2.obj");

	models[MODEL_ID_TOWER] = new ObjectModel("../assets/tower/tower.obj");
	models[MODEL_ID_TESLA] = new ObjectModel("../assets/tesla/tower_tesla.obj");
	models[MODEL_ID_RAILGUN] = new ObjectModel("../assets/railgun/tower_railgun.obj");
	models[MODEL_ID_BARRIER] = new ObjectModel("../assets/barricade/barricade_wood.obj");

	models[MODEL_ID_RESOURCE] = new ObjectModel("../assets/tree/tree.obj");
	models[MODEL_ID_RESOURCE_STONE] = new ObjectModel("../assets/crystal_rock/crystal_rock.obj");

	models[MODEL_ID_PROJECTILE] = new ObjectModel("../assets/green_projectile/green_projectile.obj");

	models[MODEL_ID_BASE] = new ObjectModel("../assets/base/base.obj");
	models[MODEL_ID_BEAR] = new ObjectModel("../assets/bear/bear.obj");
	models[MODEL_ID_BEAR_HEAD] = new ObjectModel("../assets/bear/head.obj");
	models[MODEL_ID_BEAR_LARM] = new ObjectModel("../assets/bear/larm.obj");
	models[MODEL_ID_BEAR_RARM] = new ObjectModel("../assets/bear/rarm.obj");
	models[MODEL_ID_BEAR_LLEG] = new ObjectModel("../assets/bear/lleg.obj");
	models[MODEL_ID_BEAR_RLEG] = new ObjectModel("../assets/bear/rleg.obj");
	models[MODEL_ID_BEAR_BODY] = new ObjectModel("../assets/bear/body.obj");

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

	shaders[MODEL_ID_RESOURCE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_RESOURCE_STONE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_PROJECTILE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_BASE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR_HEAD] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR_LARM] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR_RARM] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR_LLEG] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR_RLEG] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BEAR_BODY] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	//shaders[MODEL_ID_SUNGOD] = new ObjectModel("../assets/crystal_rock/crystal_rock.obj");

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
			&& incomingData.models[i].modelID != MODEL_ID_PROJECTILE
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
			newCLogs.pop();
			effect->resourceEffect(entities[target]->getPosition(), entities[target]->getModelID());
		}
		else if (newCLogs.top().damage) {
			int source = newCLogs.top().source;
			newCLogs.pop();
			if (entities[source]->getModelID() == MODEL_ID_TESLA) {
				effect->teslaAttackEffect(entities[source]->getPosition());
			}
		}
	}
	while (!newSLogs.empty()) {
		int source = newSLogs.top().source;
		int sound = newSLogs.top().sound;
		newSLogs.pop();
		if (sound == SOUND_ID_JUMP) {
			std::cout << "JUMPED\n";
			effect->teslaAttackEffect(entities[source]->getPosition());
		}
	}
	effect->update(currTime);
}

//render all active entities
void GameWorld::draw() {
	float currTime = float(glfwGetTime());
	const glm::mat4& viewProjMtx = cam->GetViewProjectMtx();
	env->draw(viewProjMtx);
	effect->draw(viewProjMtx, cam);
	for (RenderEntity* e : entities) {

		if (e->getActive()) {
			e->draw(viewProjMtx, currTime, cam);
		}
	}
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
