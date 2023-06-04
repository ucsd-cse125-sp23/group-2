#include "GameWorld.h"
float GameWorld::prevX, GameWorld::prevY,GameWorld::currX, GameWorld::currY, GameWorld::scrollY = 0;

void GameWorld::init() {


	currID = 0;
	env = new Skybox();
	env->setSkyShader(new Shader("../shaders/skybox.vert", "../shaders/skybox.frag"));
	env->setEnvShader(new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag"));

	Shader* particleShader = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	ObjectModel* particleModel = new ObjectModel("../assets/particle/testParticle.obj");
	particle = new ParticleGenerator(particleShader, particleModel, 1);

	models[MODEL_ID_CUBE] = new ObjectModel("../assets/cube/cube.obj");
	models[MODEL_ID_ROVER] = new ObjectModel("../assets/rover/rover.obj");

	models[MODEL_ID_MOB] = new ObjectModel("../assets/martian/martian.obj");
	models[MODEL_ID_MOB_TANK] = new ObjectModel("../assets/martian/martian.obj");
	models[MODEL_ID_MOB_MINI] = new ObjectModel("../assets/martian/martian.obj"); //TODO: make smol
	models[MODEL_ID_MOB_FLYING] = new ObjectModel("../assets/ufo/ufo_v2.obj");
	models[MODEL_ID_MOB_TRACTOR] = new ObjectModel("../assets/ufo/ufo_v2.obj");

	models[MODEL_ID_TOWER] = new ObjectModel("../assets/tower/tower.obj");
	models[MODEL_ID_TESLA] = new ObjectModel("../assets/tesla/tower_tesla.obj");
	models[MODEL_ID_RAILGUN] = new ObjectModel("../assets/railgun/tower_railgun.obj");
	models[MODEL_ID_BARRIER] = new ObjectModel("../assets/barricade/barricade_wood.obj");

	models[MODEL_ID_RESOURCE] = new ObjectModel("../assets/tree/tree.obj");
	models[MODEL_ID_RESOURCE_STONE] = new ObjectModel("../assets/crystal_rock/crystal_rock.obj");

	models[MODEL_ID_PROJECTILE] = new ObjectModel("../assets/laser_projectile/laser_projectile.obj");

	models[MODEL_ID_BASE] = new ObjectModel("../assets/bear/bear.obj");
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

	shaders[MODEL_ID_MOB] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_MOB_TANK] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag"); //dunno if we want new shading for the tank and minis
	shaders[MODEL_ID_MOB_MINI] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
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
}

void GameWorld::update(ServertoClientData& incomingData, int id) {
	//particle->Update(0.0f, glm::vec3(0.0f, 0.0f, 5.0f), 2, glm::vec3(1.0f));

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
			healths[i]->update(incomingData.positions[id], incomingData.positions[i], incomingData.healths[i].curHealth, incomingData.healths[i].maxHealth);
		}
		else {
			healths[i]->setActive(false);
		}
		if (incomingData.activity[i] && incomingData.models[i].modelID != MODEL_ID_NO_MODEL) {
			entities[i]->setActive(true);
			entities[i]->setModel(models[incomingData.models[i].modelID]);
			entities[i]->setShader(shaders[incomingData.models[i].modelID]);
			entities[i]->update(incomingData.positions[i], incomingData.models[i].modelOrientation);
		}
		else {
			entities[i]->setActive(false);
		}
	}

	int maxDelta = 100;
	int dx = glm::clamp((int)(currX - prevX), -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)(currY - prevY)), -maxDelta, maxDelta);
	prevX = (int)currX;
	prevY = (int)currY;

	//screen shake
	bool shakeScreen = false;
	float currHealth = incomingData.healths[id].curHealth / incomingData.healths[id].maxHealth;
	if (currHealth != playerHealth) {
		shakeScreen = true;
		playerHealth = currHealth;
	}
	bool screenShakeOn = false;
	float startTime = 0;
	float currTime = glfwGetTime();

	if (shakeScreen) {
		screenShakeOn = true;
		startTime = glfwGetTime();
	}
	if (!(screenShakeOn && currTime < (startTime + 3))) {
		screenShakeOn = false;
		shakeScreen = false;
	}
	cam->update(entities[id]->getPosition(), dx, dy, scrollY, screenShakeOn, currTime);
}

//render all active entities
void GameWorld::draw() {
	float currTime = float(glfwGetTime());
	const glm::mat4& viewProjMtx = cam->GetViewProjectMtx();
	//env->draw(viewProjMtx);
	particle->draw(viewProjMtx, currTime, cam);
	/*
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
	*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void GameWorld::cursor_callback(GLFWwindow* window, double cX, double cY) {
	currX = cX;
	currY = cY;
}
