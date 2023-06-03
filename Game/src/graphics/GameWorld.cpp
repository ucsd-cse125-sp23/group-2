
	#include "GameWorld.h"
	float GameWorld::prevX, GameWorld::prevY, GameWorld::currX, GameWorld::currY, GameWorld::scrollY = 0;
	std::array <GUIElement*, NUM_GUI> guis;
	int selected;
	int depth;
	bool menuOn;
	float spawntimer;
	float moneyRes;
	float woodRes;
	float stoneRes;
	float points;
	float enemiesKilled;
	float towersBuilt;
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
	models[MODEL_ID_BASE] = new ObjectModel("../assets/cube/cube.obj");
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

	for (int i = 0; i < MAX_ENTITIES; i++) {
		entities[i] = new RenderEntity(i);
		AABBs[i] = new Cube();
		healths[i] = new HealthBar(healthShader, healthModel);
	}

	for (int i = 0; i < NUM_GUI; i++) {
		guis[i] = new GUIElement();
	}
	cam = new Camera();
	GUI_Init();
}

void GameWorld::GUI_Init() {
	selected = 0;
	depth = 0;
	menuOn = false;
	//continue
	continue_Btn = guis[0];
	continue_Btn->SetHidden(true);
	continue_Btn->SetName("continue");
	continue_Btn->SetPosition(glm::vec3(0.0f, 0.30f, 0.0f));
	continue_Btn->SetSize(glm::vec2(0.75f, 0.25f));
	continue_Btn->SetTexture("../assets/gui/Buttons/continue.jpg");
	continue_Btn->SetTransparency(1.0);
	guis[0] = continue_Btn;
	//options
	options_Btn = guis[1];
	options_Btn->SetHidden(true);
	options_Btn->SetName("options");
	options_Btn->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	options_Btn->SetSize(glm::vec2(0.75f, 0.25f));
	options_Btn->SetTexture("../assets/gui/Buttons/options.jpg");
	options_Btn->SetTransparency(1.0);
	guis[1] = options_Btn;
	//quit
	quit_Btn = guis[2];
	quit_Btn->SetHidden(true);
	quit_Btn->SetName("quit");
	quit_Btn->SetPosition(glm::vec3(0.0f, -0.30f, 0.0f));
	quit_Btn->SetSize(glm::vec2(0.75f, 0.25f));
	quit_Btn->SetTexture("../assets/gui/Buttons/quit.jpg");
	quit_Btn->SetTransparency(1.0);
	guis[2] = quit_Btn;

	//back
	back_Btn = guis[3];
	back_Btn->SetHidden(true);
	back_Btn->SetName("back");
	back_Btn->SetPosition(glm::vec3(0.0f, -0.3f, 0.03f));
	back_Btn->SetSize(glm::vec2(0.75f, 0.25f));
	back_Btn->SetTexture("../assets/gui/Buttons/back.jpg");
	back_Btn->SetTransparency(1.0);
	guis[3] = back_Btn;

	//Volume
	volume_Sldr = guis[4];
	volume_Sldr->SetHidden(true);
	volume_Sldr->SetName("volume");
	volume_Sldr->SetPosition(glm::vec3(-0.5f, 0.0f, 0.0f));
	volume_Sldr->SetSize(glm::vec2(0.75f, 0.25f));
	volume_Sldr->SetTexture("../assets/gui/Buttons/volume.jpg");
	volume_Sldr->SetTransparency(1.0);
	 
	guis[4] = volume_Sldr;
	volume_Sldr_Back = guis[5];
	volume_Sldr_Back->SetHidden(true);
	volume_Sldr_Back->SetName("volume_Sldr_Back");
	volume_Sldr_Back->SetPosition(glm::vec3(0.5f, 0.0f, 0.2f));
	volume_Sldr_Back->SetSize(glm::vec2(0.75f, 0.1f));
	volume_Sldr_Back->SetTexture("../assets/gui/Buttons/volume_Sldr_Back.jpg");
	volume_Sldr_Back->SetTransparency(0.0);
	guis[5] = volume_Sldr_Back;
	volume_Sldr_Front = guis[6];
	volume_Sldr_Front->SetHidden(true);
	volume_Sldr_Front->SetName("volume_Sldr_Front");
	volume_Sldr_Front->SetPosition(glm::vec3(0.5f, 0.0f, 0.1f));
	volume_Sldr_Front->SetSize(glm::vec2(0.75f, 0.1f));
	volume_Sldr_Front->SetTexture("../assets/gui/Buttons/volume_Sldr_Front.jpg");
	volume_Sldr_Front->SetTransparency(1.0);
	guis[6] = volume_Sldr_Front;

	//Health
	health_Bar = guis[7];
	health_Bar->SetHidden(true);
	health_Bar->SetName("health");
	health_Bar->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	health_Bar->SetSize(glm::vec2(1.0f, 1.0f));
	health_Bar->SetTexture("../assets/gui/Buttons/health.jpg");
	health_Bar->SetTransparency(1.0);
	guis[7] = health_Bar;
	health_Bar_Back = guis[7];
	health_Bar_Back->SetHidden(true);
	health_Bar_Back->SetName("health_back");
	health_Bar_Back->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	health_Bar_Back->SetSize(glm::vec2(1.0f, 1.0f));
	health_Bar_Back->SetTexture("../assets/gui/Buttons/health_back.jpg");
	health_Bar_Back->SetTransparency(1.0);
	guis[8] = health_Bar_Back;
	health_Bar_Front = guis[9];
	health_Bar_Front->SetHidden(true);
	health_Bar_Front->SetName("health_front");
	health_Bar_Front->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	health_Bar_Front->SetSize(glm::vec2(1.0f, 1.0f));
	health_Bar_Front->SetTexture("../assets/gui/Buttons/health_front.jpg");
	health_Bar_Front->SetTransparency(1.0);
	guis[9] = health_Bar_Front;

	blank = guis[10];
	blank->SetHidden(true);
	blank->SetName("health_front");
	blank->SetPosition(glm::vec3(0.5f, 0.0f, 0.0f));
	blank->SetSize(glm::vec2(0.75f, 0.25f));
	blank->SetTexture("../assets/gui/Buttons/blank.jpg");
	blank->SetTransparency(1.0);
	guis[10] = blank;
	
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
		if (incomingData.activity[i]) {
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
	bool shakeScreen = false;
	bool screenShakeOn = false;
	float startTime = 0;
	float currTime = glfwGetTime();

	//should be bool from server
	if (shakeScreen) {
		screenShakeOn = true;
		startTime = glfwGetTime();
	}
	if (screenShakeOn && currTime < (startTime + 3)) {

	}
	else {
		screenShakeOn = false;
		shakeScreen = false;
	}
	cam->update(entities[id]->getPosition(), dx, dy, scrollY, screenShakeOn, currTime);
}

//render all active entities
void GameWorld::draw(Shader* guiShader, float wWidth, float wHeight) {

	width = wWidth;
	height = wHeight;

	float currTime = float(glfwGetTime());
	const glm::mat4& viewProjMtx = cam->GetViewProjectMtx();
	env->draw(viewProjMtx);

	for (RenderEntity* e : entities) {

		if (e->getActive()) {
			e->draw(viewProjMtx, currTime);
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
	for (GUIElement* gui : guis) {
		if (!gui->IsHidden()) {
			gui->draw(viewProjMtx, guiShader);
		}

	}
}

void GameWorld::cursor_callback(GLFWwindow* window, double cX, double cY) {
	if (menuOn) {

	}
	else {

		currX = cX;
		currY = cY;
	}

}