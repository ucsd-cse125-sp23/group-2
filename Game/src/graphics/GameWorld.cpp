
#include "GameWorld.h"
float GameWorld::prevX, GameWorld::prevY, GameWorld::currX, GameWorld::currY, GameWorld::scrollY = 0;
std::array <GUIElement*, NUM_GUI> guis;
int selected;
int depth;
bool menuOn;
int spawntimer=0;
int moneyRes=0;
int woodRes=0;
int stoneRes=0;
int points=0;
int enemiesKilled=0;
int towersBuilt=0;

Text* spawntimerT;
Text* moneyResT;
Text* woodResT;
Text* stoneResT;
Text* pointsT;
Text* enemiesKilledT;
Text* towersBuiltT;
char str[65536];

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

	for (int i = 0; i < NUM_GUI; i++) {
		guis[i] = new GUIElement();
	}
	GUI_Init();
}
void GameWorld::GUI_Init() {
	text = new Text("../assets/font.ttf");
	spawntimerT = new Text("../assets/font.ttf");
	moneyResT = new Text("../assets/font.ttf");
	woodResT = new Text("../assets/font.ttf");
	stoneResT = new Text("../assets/font.ttf");
	pointsT = new Text("../assets/font.ttf");
	enemiesKilledT = new Text("../assets/font.ttf");
	towersBuiltT = new Text("../assets/font.ttf");
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
	volume_Sldr_Back->SetPosition(glm::vec3(0.5f, 0.0f, -0.1f));
	volume_Sldr_Back->SetSize(glm::vec2(0.60f, 0.1f));
	volume_Sldr_Back->SetTexture("../assets/gui/Buttons/volume_Sldr_Back.jpg");
	volume_Sldr_Back->SetTransparency(0.0);
	guis[5] = volume_Sldr_Back;
	volume_Sldr_Front = guis[6];
	volume_Sldr_Front->SetHidden(true);
	volume_Sldr_Front->SetName("volume_Sldr_Front");
	volume_Sldr_Front->SetPosition(glm::vec3(0.5f, 0.0f	, -0.6f));
	volume_Sldr_Front->SetSize(glm::vec2(0.60f, 0.1f));
	volume_Sldr_Front->SetTexture("../assets/gui/Buttons/volume_Sldr_Front.jpg");
	volume_Sldr_Front->SetTransparency(1.0);
	guis[6] = volume_Sldr_Front;

	//Health
	health_Bar = guis[7];
	health_Bar->SetHidden(false);
	health_Bar->SetName("health");
	health_Bar->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	health_Bar->SetSize(glm::vec2(0.5f, 0.5f));
	health_Bar->SetTexture("../assets/gui/Buttons/crosshair.png");
	health_Bar->SetTransparency(1.0);
	guis[7] = health_Bar;
	health_Bar_Back = guis[7];
	health_Bar_Back->SetHidden(false);
	health_Bar_Back->SetName("health_back");
	health_Bar_Back->SetPosition(glm::vec3(-0.72f, -0.905f, 0.0f));
	health_Bar_Back->SetSize(glm::vec2(0.38f, 0.05f));
	health_Bar_Back->SetTexture("../assets/gui/Buttons/health_back.jpg");
	health_Bar_Back->SetTransparency(1.0);
	guis[8] = health_Bar_Back;
	health_Bar_Front = guis[9];
	health_Bar_Front->SetHidden(false);
	health_Bar_Front->SetName("health_front");
	health_Bar_Front->SetPosition(glm::vec3(-0.72f, -0.905f, -0.2f));
	health_Bar_Front->SetSize(glm::vec2(0.38f, 0.05f));
	health_Bar_Front->SetTexture("../assets/gui/Buttons/health_front.jpg");
	health_Bar_Front->SetTransparency(1.0);
	guis[9] = health_Bar_Front;

	blank = guis[10];
	blank->SetHidden(true);
	blank->SetName("blank");
	blank->SetPosition(glm::vec3(0.5f, 0.0f, 0.0f));
	blank->SetSize(glm::vec2(0.75f, 0.25f));
	blank->SetTexture("../assets/gui/Buttons/blank.jpg");
	blank->SetTransparency(1.0);
	guis[10] = blank;

	gui = guis[11];
	gui->SetHidden(false);
	gui->SetName("gui");
	gui->SetPosition(glm::vec3(0.0f, -0.90f, 0.3f));
	gui->SetSize(glm::vec2(2.0f, 0.2f));
	gui->SetTexture("../assets/gui/Buttons/hud.jpg");
	gui->SetTransparency(1.0);
	guis[11] = gui;

	black = guis[12];
	black->SetHidden(true);
	black->SetName("black");
	black->SetPosition(glm::vec3(0.0f, 0.0f, -0.7f));
	black->SetSize(glm::vec2(2.0f, 2.0f));
	black->SetTexture("../assets/gui/Buttons/black.jpg");
	black->SetTransparency(1.0);
	guis[12] = black;

	you_win = guis[13];
	you_win->SetHidden(true);
	you_win->SetName("you_win");
	you_win->SetPosition(glm::vec3(0.0f, 0.5f, -0.8f));
	you_win->SetSize(glm::vec2(0.75f, 0.25f));
	you_win->SetTexture("../assets/gui/Buttons/victory.jpg");
	you_win->SetTransparency(1.0);
	guis[13] = you_win;
	
	you_lose = guis[14];
	you_lose->SetHidden(true);
	you_lose->SetName("black");
	you_lose->SetPosition(glm::vec3(0.0f, 0.5f, -0.8f));
	you_lose->SetSize(glm::vec2(0.75f, 0.25f));	
	you_lose->SetTexture("../assets/gui/Buttons/you_died.jpg");
	you_lose->SetTransparency(1.0);
	guis[14] = you_lose;

	new_game = guis[15];
	new_game->SetHidden(true);
	new_game->SetName("new_game");
	new_game->SetPosition(glm::vec3(0.0f, 0.0f, -0.8f));
	new_game->SetSize(glm::vec2(0.75f, 0.25f));
	new_game->SetTexture("../assets/gui/Buttons/new_game.jpg");
	new_game->SetTransparency(1.0);
	guis[15] = new_game;

	quit_Btn1 = guis[16];
	quit_Btn1->SetHidden(true);
	quit_Btn1->SetName("quit");
	quit_Btn1->SetPosition(glm::vec3(0.0f, -0.30f, -0.8f));
	quit_Btn1->SetSize(glm::vec2(0.75f, 0.25f));
	quit_Btn1->SetTexture("../assets/gui/Buttons/quit.jpg");
	quit_Btn1->SetTransparency(1.0);
	guis[16] = quit_Btn1;


	gui = guis[17];
	gui->SetHidden(false);
	gui->SetName("gui");
	gui->SetPosition(glm::vec3(0.0f, 0.90f, 0.3f));
	gui->SetSize(glm::vec2(2.0f, 0.2f));
	gui->SetTexture("../assets/gui/Buttons/hud_top.jpg");
	gui->SetTransparency(1.0);
	guis[17] = gui;

	/*= guis[];
	->SetHidden(true);
	->SetName("black");
	->SetPosition(glm::vec3(0.0f, 0.0f, -0.8f));
	->SetSize(glm::vec2(0.75f, 0.25f));
	->SetTexture("../assets/gui/Buttons/.jpg");
	->SetTransparency(1.0);
	guis[] = ;*/

	crosshair  = guis[18];
	crosshair->SetHidden(false);
	crosshair->SetName("black");
	crosshair->SetPosition(glm::vec3(0.0f, 0.0f, -0.8f));
	crosshair->SetSize(glm::vec2(0.05f, 0.08f));
	crosshair->SetTexture("../assets/gui/Buttons/crosshair.png");
	crosshair->SetTransparency(0.5f);
	guis[18] = crosshair;

	
}

void updateLabels() {
	sprintf(str, "%d", moneyRes);
	moneyResT->RenderText(str, 2040.0f, 35.0f, 1.5f, glm::vec3(10.0, 10.0f, 10.0f));
	
	sprintf(str, "%d", woodRes);
	woodResT->RenderText(str, 1400.0f, 35.0f, 1.5f, glm::vec3(10.0, 10.0f, 10.0f));
	
	sprintf(str, "%d", stoneRes);
	stoneResT->RenderText(str, 770.0f, 35.0f, 1.5f, glm::vec3(10.0, 10.0f, 10.0f));

	sprintf(str, "%d", spawntimer);
	strcat(str, " seconds");
	spawntimerT->RenderText(str, 1300.0f, 1330.0f, 1.5f, glm::vec3(10.0, 10.0f, 10.0f));
	
	sprintf(str, "%d", points);
	strcat(str, " points");
	pointsT->RenderText(str, 2050.0f, 1330.0f, 1.5f, glm::vec3(10.0, 10.0f, 10.0f));
	
	sprintf(str, "%d", enemiesKilled);
	strcat(str, " kills");
	enemiesKilledT->RenderText(str, 800.0f, 1330.0f, 1.5f, glm::vec3(10.0, 10.0f, 10.0f));
	
	sprintf(str, "%d", towersBuilt);
	strcat(str, " towers");
	towersBuiltT->RenderText(str, 70.0f, 1330.0f, 1.5f, glm::vec3(10.0, 10.0f, 10.0f));
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
			healths[i]->update(cam->getCameraPosition(), incomingData.positions[i] + glm::vec3(0.0f, incomingData.colliders[i].AABB.y, 0.0f),
				incomingData.healths[i].curHealth, incomingData.healths[i].maxHealth);
		}
		else {
			healths[i]->setActive(false);
		}
		if (incomingData.activity[i] && incomingData.models[i].modelID != MODEL_ID_NO_MODEL) {
			entities[i]->setActive(true);
			entities[i]->setModel(models[incomingData.models[i].modelID]);
			entities[i]->setShader(shaders[incomingData.models[i].modelID]);
			entities[i]->update(incomingData.positions[i], incomingData.models[i].modelOrientation, incomingData.models[i].scale);
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
}

void GameWorld::draw(Shader* guiShader, float wWidth, float wHeight) {

	width = wWidth;
	height = wHeight;
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

	for (GUIElement* gui : guis) {
		if (!gui->IsHidden()) {
			gui->draw(viewProjMtx, guiShader);
		}
	}
	updateLabels();

}
void GameWorld::cursor_callback(GLFWwindow* window, double cX, double cY) {
	if (!menuOn) {
		currX = cX;
		currY = cY;
	}
}