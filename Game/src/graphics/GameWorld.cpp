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
	models[MODEL_ID_MOB_FLYING] = new ObjectModel("../assets/ufo/ufo_v2.obj");
	models[MODEL_ID_TOWER] = new ObjectModel("../assets/tower/tower.obj");
	models[MODEL_ID_BASE] = new ObjectModel("../assets/base/test_base.obj");
	//replace once models are done
	models[MODEL_ID_RESOURCE] = new ObjectModel("../assets/tree/tree.obj");
	models[MODEL_ID_PROJECTILE] = new ObjectModel("../assets/laser_projectile/laser_projectile.obj");

	shaders[MODEL_ID_CUBE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_ROVER] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_MOB] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_MOB_FLYING] = new Shader("../shaders/ufo_shader.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TOWER] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BASE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	//replace once models are done
	shaders[MODEL_ID_RESOURCE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_PROJECTILE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

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

	for (int i = 0; i < incomingData.activity.size(); i++) {

		if (incomingData.activity[i]) {
			if (incomingData.models[i].renderCollider) {
				AABBs[i]->setActive(true);
				AABBs[i]->update(incomingData.positions[i], incomingData.colliders[i].AABB);
			}
			else {
				AABBs[i]->setActive(false);
			}
		}

		//if active and should render health bar
		if (incomingData.activity[i]
			&& i != id
			&& incomingData.models[i].modelID != MODEL_ID_PROJECTILE) {
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
	env->draw(viewProjMtx);

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

void GameWorld::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:

			break;
		default: break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			
			break;
		default: break;
		}
	}
}