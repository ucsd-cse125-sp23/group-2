
#include "GameWorld.h"
float GameWorld::prevX, GameWorld::prevY, GameWorld::currX, GameWorld::currY, GameWorld::scrollY = 0;
std::array <GUIElement*, NUM_GUI> guis;
int selected;
int depth;
bool menuOn;

//Render when dead
int spawntimer=0;


//High priority to render (esp when building/upgrrading)
int moneyRes=0;
int woodRes=0;
int stoneRes=0;

//Low priority to render
int points=0;
int enemiesKilled=0;
int towersBuilt=0;

//High prioritry to render
int wavetimer=0; //Do not render as a bar (render as a number since waves have incosistent timings)
int maxwavetimer=0;

//High priority
int curWave = 0;
int numWaves = WAVE_COUNT;

//Medium priority 
float basehealth = 0;
float maxbasehealth = 0;

//Nice to render but not required (only render when non-zero) should be a hidden element otherwise (This is the timer for the current power up, maybe display as a temporary icon using power
//up icons that are already made?
int poweruptimer = 0;


//Only render when one of these three is non-zero should be a hidden element otherwise (These are the costs of building/upgrading and are only non-zero when the player has selected a
//tower to build or upgrade. Maybe a pop-up minus ressource symbol above the regular resource counters
int moneyCost = 0;
int woodCost = 0;
int stoneCost = 0;
int score1;
int score2; 
int score3;
int score4;
int max_score;
bool scoreShown;
//not necessary to render (is the time before player is allowed to complete another build or upgrade action)
int actionCooldown = 0;

//Change crosshair based on state values in playerstate struct
//If state == PlayerState::Upgrading vs if state == PlayerState::Build/PlayerState::Default/PlayerState::Attack
State state = 0;






Text* spawntimerT;
Text* moneyResT;
Text* woodResT;
Text* stoneResT;

Text* moneyCostT;
Text* woodCostT;
Text* stoneCostT;

Text* pointsT;
Text* enemiesKilledT;
Text* towersBuiltT;
Text* waveCounter;

Text* buildCost;

Text* p1;
Text* p2;
Text* p3;
Text* p4;

char str[65536];
char tmp_str[65536];

void GameWorld::init(GLFWwindow* window) {

	guiProgram = new Shader("../shaders/gui.vert", "shaders/gui.frag");
	loading(window, "../assets/screens/Title0.png");

	currID = 0;
	env = new Skybox();
	env->setSkyShader(new Shader("../shaders/skybox.vert", "../shaders/skybox.frag"));
	env->setEnvShader(new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag"));
	env->setPortalShader(new Shader("../shaders/portalshader.vert", "../shaders/portalshader.frag"));

	models[MODEL_ID_CUBE] = new ObjectModel("../assets/cube/cube.obj");
	models[MODEL_ID_ROVER_RED] = new ObjectModel("../assets/rover/rover_red.obj");
	models[MODEL_ID_ROVER_BLUE] = new ObjectModel("../assets/rover/rover_blue.obj");
	models[MODEL_ID_ROVER_GREEN] = new ObjectModel("../assets/rover/rover_green.obj");
	models[MODEL_ID_ROVER_PURPLE] = new ObjectModel("../assets/rover/rover_purple.obj");

	loading(window, "../assets/screens/Title10.png");

	models[MODEL_ID_MOB] = new ObjectModel("../assets/martian/martian.obj");
	models[MODEL_ID_MOB_TANK] = new ObjectModel("../assets/martian/martian_tank.obj");
	models[MODEL_ID_MOB_MINI] = new ObjectModel("../assets/martian/martian_fast.obj");
	models[MODEL_ID_MOB_FLYING] = new ObjectModel("../assets/ufo/ufo_v2.obj");
	models[MODEL_ID_MOB_TRACTOR] = new ObjectModel("../assets/ufo/ufo_abductor.obj");

	loading(window, "../assets/screens/Title20.png");

	models[MODEL_ID_TOWER] = new ObjectModel("../assets/tower/tower.obj");
	models[MODEL_ID_TESLA] = new ObjectModel("../assets/tesla/tower_tesla.obj");
	models[MODEL_ID_RAILGUN] = new ObjectModel("../assets/railgun/tower_railgun.obj");
	models[MODEL_ID_BARRIER] = new ObjectModel("../assets/barricade/barricade_wood.obj");
	models[MODEL_ID_TOWER_INVALID] = new ObjectModel("../assets/tower/Tower_invalid.obj");
	models[MODEL_ID_TESLA_INVALID] = new ObjectModel("../assets/tesla/tower_tesla_invalid.obj");
	models[MODEL_ID_RAILGUN_INVALID] = new ObjectModel("../assets/railgun/tower_railgun_invalid.obj");
	models[MODEL_ID_BARRIER_INVALID] = new ObjectModel("../assets/barricade/barricade_wood_invalid.obj");
	models[MODEL_ID_TOWER_L2] = new ObjectModel("../assets/tower/Tower_L2.obj");
	models[MODEL_ID_TESLA_L2] = new ObjectModel("../assets/tesla/tower_tesla_L2.obj");
	models[MODEL_ID_RAILGUN_L2] = new ObjectModel("../assets/railgun/tower_railgun_L2.obj");
	models[MODEL_ID_BARRIER_L2] = new ObjectModel("../assets/barricade/barricade_wood_L2.obj");

	models[MODEL_ID_TOWER_L3] = new ObjectModel("../assets/tower/Tower_L3.obj");
	models[MODEL_ID_TESLA_L3] = new ObjectModel("../assets/tesla/tower_tesla_L3.obj");
	models[MODEL_ID_RAILGUN_L3] = new ObjectModel("../assets/railgun/tower_railgun_L3.obj");
	models[MODEL_ID_BARRIER_L3] = new ObjectModel("../assets/barricade/barricade_wood_L3.obj");

	models[MODEL_ID_TOWER_L4] = new ObjectModel("../assets/tower/Tower_L4.obj");
	models[MODEL_ID_TESLA_L4] = new ObjectModel("../assets/tesla/tower_tesla_L4.obj");
	models[MODEL_ID_RAILGUN_L4] = new ObjectModel("../assets/railgun/tower_railgun_L4.obj");
	models[MODEL_ID_BARRIER_L4] = new ObjectModel("../assets/barricade/barricade_wood_L4.obj");

	loading(window, "../assets/screens/Title30.png");

	models[MODEL_ID_RESOURCE] = new ObjectModel("../assets/tree/tree.obj");
	models[MODEL_ID_RESOURCE_1] = new ObjectModel("../assets/tree/tree1.obj");
	models[MODEL_ID_RESOURCE_2] = new ObjectModel("../assets/tree/tree2.obj");
	models[MODEL_ID_RESOURCE_STONE] = new ObjectModel("../assets/crystal_rock/crystal_rock.obj");

	models[MODEL_ID_PROJECTILE_ROVER] = new ObjectModel("../assets/projectiles/projectile_rover.obj");
	models[MODEL_ID_PROJECTILE_MARTIAN] = new ObjectModel("../assets/projectiles/projectile_martian.obj");
	models[MODEL_ID_PROJECTILE_SPREAD] = new ObjectModel("../assets/projectiles/projectile_spreadshot.obj");
	models[MODEL_ID_PROJECTILE_RAPID] = new ObjectModel("../assets/projectiles/projectile_rapidfire.obj");


	models[MODEL_ID_BASE] = new ObjectModel("../assets/base/base.obj");
	loading(window, "../assets/screens/Title40.png");

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

	loading(window, "../assets/screens/Title50.png");

	//models[MODEL_ID_SUNGOD] = new ObjectModel("../assets/crystal_rock/crystal_rock.obj");

	shaders[MODEL_ID_CUBE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_ROVER_RED] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_ROVER_BLUE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_ROVER_GREEN] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_ROVER_PURPLE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_MOB] = new Shader("../shaders/martian_shader.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_MOB_TANK] = new Shader("../shaders/martian_shader.vert", "../shaders/model_loading.frag"); //dunno if we want new shading for the tank and minis
	shaders[MODEL_ID_MOB_MINI] = new Shader("../shaders/martian_shader.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_MOB_FLYING] = new Shader("../shaders/ufo_shader.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_MOB_TRACTOR] = new Shader("../shaders/ufo_shader.vert", "../shaders/ufo_shader.frag");

	loading(window, "../assets/screens/Title60.png");

	shaders[MODEL_ID_TOWER] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_RAILGUN] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TESLA] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BARRIER] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TOWER_L2] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_RAILGUN_L2] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TESLA_L2] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BARRIER_L2] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_TOWER_L3] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	
	shaders[MODEL_ID_RAILGUN_L3] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TESLA_L3] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");	
	shaders[MODEL_ID_BARRIER_L3] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_TOWER_L4] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_RAILGUN_L4] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TESLA_L4] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BARRIER_L4] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_TOWER_INVALID] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_RAILGUN_INVALID] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_TESLA_INVALID] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_BARRIER_INVALID] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_RESOURCE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_RESOURCE_1] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_RESOURCE_2] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_RESOURCE_STONE] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	shaders[MODEL_ID_PROJECTILE_ROVER] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_PROJECTILE_MARTIAN] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_PROJECTILE_SPREAD] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");
	shaders[MODEL_ID_PROJECTILE_RAPID] = new Shader("../shaders/model_loading.vert", "../shaders/model_loading.frag");

	loading(window, "../assets/screens/Title70.png");

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

	loading(window, "../assets/screens/Title80.png");

	healthShader = new Shader("../shaders/shader.vert", "../shaders/shader.frag");
	ObjectModel* healthModel = new ObjectModel("../assets/cube/cube.obj");
	playerHealth = 1.0f;
	for (int i = 0; i < MAX_ENTITIES; i++) {
		entities[i] = new RenderEntity(i);
		AABBs[i] = new Cube(shaders[MODEL_ID_CUBE], models[MODEL_ID_CUBE]);
		healths[i] = new HealthBar(healthShader, healthModel);
	}

	loading(window, "../assets/screens/Title90.png");
	cam = new Camera();
	shakeScreen = false;
	screenShakeOn = false;
	startTime = 0;
	effect = new EffectSystem();
	for (int i = 0; i < NUM_GUI; i++) {
		guis[i] = new GUIElement();
	}

	loading(window, "../assets/screens/Title100.png");
	GUI_Init();

	loading(window, "../assets/screens/Space.png");
}




void GameWorld::loading(GLFWwindow* window, char* path) {

	glfwMakeContextCurrent(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	stbi_set_flip_vertically_on_load(true);
	GUIElement* loadingScreen = new GUIElement();
	loadingScreen->SetHidden(false);
	loadingScreen->SetName("loading screen");
	loadingScreen->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	loadingScreen->SetSize(glm::vec2(2.0f, 1.6f));
	loadingScreen->SetTexture(path);
	loadingScreen->SetTransparency(1.0);
	loadingScreen->draw(glm::mat4(1.0), guiProgram);

	glfwPollEvents();
	glfwSwapBuffers(window);
	stbi_set_flip_vertically_on_load(false);
	// Loading Screen
}

void GameWorld::GUI_Init() {
	text = new Text("../assets/font.ttf");
	spawntimerT = new Text("../assets/font.ttf");
	moneyResT = new Text("../assets/font.ttf");
	woodResT = new Text("../assets/font.ttf");
	stoneResT = new Text("../assets/font.ttf");
	moneyCostT = new Text("../assets/font.ttf");
	woodCostT = new Text("../assets/font.ttf");
	stoneCostT = new Text("../assets/font.ttf");
	pointsT = new Text("../assets/font.ttf");
	enemiesKilledT = new Text("../assets/font.ttf");
	towersBuiltT = new Text("../assets/font.ttf");
	waveCounter = new Text("../assets/font.ttf");
	p1 = new Text("../assets/font.ttf");
	p2 = new Text("../assets/font.ttf");
	p3 = new Text("../assets/font.ttf");
	p4 = new Text("../assets/font.ttf");
	selected = 0;
	depth = 0;
	menuOn = false;
	//continue
	continue_Btn = guis[0];
	continue_Btn->SetHidden(true);
	continue_Btn->SetName("continue");
	continue_Btn->SetPosition(glm::vec3(0.0f, 0.30f, -0.2f));
	continue_Btn->SetSize(glm::vec2(0.75f, 0.4f));
	continue_Btn->SetTexture("../assets/gui/Buttons/continue.png");
	continue_Btn->SetTransparency(1.0);
	guis[0] = continue_Btn;
	//options
	options_Btn = guis[1];
	options_Btn->SetHidden(true);
	options_Btn->SetName("options");
	options_Btn->SetPosition(glm::vec3(0.0f, 0.0f, -0.2f));
	options_Btn->SetSize(glm::vec2(0.75f, 0.4f));
	options_Btn->SetTexture("../assets/gui/Buttons/options.png");
	options_Btn->SetTransparency(1.0);
	guis[1] = options_Btn;
	//quit
	quit_Btn = guis[2];
	quit_Btn->SetHidden(true);
	quit_Btn->SetName("quit");
	quit_Btn->SetPosition(glm::vec3(0.0f, -0.30f, -0.2f));
	quit_Btn->SetSize(glm::vec2(0.75f, 0.4f));
	quit_Btn->SetTexture("../assets/gui/Buttons/quit.png");
	quit_Btn->SetTransparency(1.0);
	guis[2] = quit_Btn;

	//back
	back_Btn = guis[3];
	back_Btn->SetHidden(true);
	back_Btn->SetName("back");
	back_Btn->SetPosition(glm::vec3(0.0f, -0.3f, 0.03f));
	back_Btn->SetSize(glm::vec2(0.75f, 0.4f));
	back_Btn->SetTexture("../assets/gui/Buttons/back.png");
	back_Btn->SetTransparency(1.0);
	guis[3] = back_Btn;

	//Volume
	volume_Sldr = guis[4];
	volume_Sldr->SetHidden(true);
	volume_Sldr->SetName("volume");
	volume_Sldr->SetPosition(glm::vec3(-0.5f, 0.0f, 0.0f));
	volume_Sldr->SetSize(glm::vec2(0.75f, 0.4f));
	volume_Sldr->SetTexture("../assets/gui/Buttons/volume.png");
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
	volume_Sldr_Front->SetPosition(glm::vec3(0.5f, 0.0f	, -0.4f));
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
	blank->SetSize(glm::vec2(0.75f, 0.4f));
	blank->SetTexture("../assets/gui/Buttons/blank.png");
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
	black->SetPosition(glm::vec3(0.0f, 0.0f, -0.6f));
	black->SetSize(glm::vec2(2.0f, 2.0f));
	black->SetTexture("../assets/gui/Buttons/black.jpg");
	black->SetTransparency(1.0);
	guis[12] = black;

	you_win = guis[13];
	you_win->SetHidden(true);
	you_win->SetName("you_win");
	you_win->SetPosition(glm::vec3(0.0f, 0.5f, -0.7f));
	you_win->SetSize(glm::vec2(0.75f, 0.4f));
	you_win->SetTexture("../assets/gui/Buttons/victory.png");
	you_win->SetTransparency(1.0);
	guis[13] = you_win;
	
	you_lose = guis[14];
	you_lose->SetHidden(true);
	you_lose->SetName("black");
	you_lose->SetPosition(glm::vec3(0.0f, 0.5f, -0.7f));
	you_lose->SetSize(glm::vec2(0.75f, 0.4f));	
	you_lose->SetTexture("../assets/gui/Buttons/game_over.png");
	you_lose->SetTransparency(1.0);
	guis[14] = you_lose;

	new_game = guis[15];
	new_game->SetHidden(true);
	new_game->SetName("new_game");
	new_game->SetPosition(glm::vec3(0.0f, 0.0f, -0.8f));
	new_game->SetSize(glm::vec2(0.75f, 0.4f));
	new_game->SetTexture("../assets/gui/Buttons/new_game.png");
	new_game->SetTransparency(1.0);
	guis[15] = new_game;

	quit_Btn1 = guis[16];
	quit_Btn1->SetHidden(true);
	quit_Btn1->SetName("quit");
	quit_Btn1->SetPosition(glm::vec3(0.0f, -0.30f, -0.8f));
	quit_Btn1->SetSize(glm::vec2(0.75f, 0.4f));
	quit_Btn1->SetTexture("../assets/gui/Buttons/quit.png");
	quit_Btn1->SetTransparency(1.0);
	guis[16] = quit_Btn1;


	gui = guis[17];
	gui->SetHidden(false);
	gui->SetName("gui");
	gui->SetPosition(glm::vec3(0.0f, 0.80f, 0.3f));
	gui->SetSize(glm::vec2(2.0f, 0.4f));
	gui->SetTexture("../assets/gui/Buttons/hud_top.png");
	gui->SetTransparency(1.0);
	guis[17] = gui;

	/*= guis[];
	->SetHidden(true);
	->SetName("black");
	->SetPosition(glm::vec3(0.0f, 0.0f, -0.8f));
	->SetSize(glm::vec2(0.75f, 0.4f));
	->SetTexture("../assets/gui/Buttons/.png");
	->SetTransparency(1.0);
	guis[] = ;*/

	crosshair  = guis[18];
	crosshair->SetHidden(false);
	crosshair->SetName("black");
	crosshair->SetPosition(glm::vec3(0.0f, 0.0f, -0.1f));
	crosshair->SetSize(glm::vec2(0.05f, 0.08f));
	crosshair->SetTexture("../assets/gui/Buttons/crosshair.png");
	crosshair->SetTransparency(0.5f);
	guis[18] = crosshair;

	towerH = guis[19];
	towerH->SetHidden(true);
	towerH->SetName("tower");
	towerH->SetPosition(glm::vec3(0.425f, 0.96f, -0.1f));
	towerH->SetSize(glm::vec2(0.05f, 0.06f));
	towerH->SetTexture("../assets/gui/Buttons/towers.png");
	towerH->SetTransparency(1.0);
	guis[19] = towerH;
	baseH= guis[20];
	baseH->SetHidden(false);
	baseH->SetName("baseH");
	baseH->SetPosition(glm::vec3(0.425f, 0.94f, -0.1f));
	baseH->SetSize(glm::vec2(0.10f, 0.12f));
	baseH->SetTexture("../assets/gui/Buttons/base.png");
	baseH->SetTransparency(1.0);
	guis[20] = baseH;
	base_health = guis[21];
	base_health->SetHidden(false);
	base_health->SetName("base_health");
	base_health->SetPosition(glm::vec3(0.575f, 0.94f, -0.1f));
	base_health->SetSize(glm::vec2(0.2f, 0.04f));
	base_health->SetTexture("../assets/gui/Buttons/health_front.jpg");
	base_health->SetTransparency(1.0);
	guis[21] = base_health;
	tower_health= guis[22];
	tower_health->SetHidden(true);
	tower_health->SetName("tower_health");
	tower_health->SetPosition(glm::vec3(0.55f, 0.96f, -0.1f));
	tower_health->SetSize(glm::vec2(0.2f, 0.03f));
	tower_health->SetTexture("../assets/gui/Buttons/health_front.jpg");
	tower_health->SetTransparency(1.0);
	guis[22] = tower_health;
	
	death = guis[23];
	death->SetHidden(true);
	death->SetName("death");
	death->SetPosition(glm::vec3(0.0f, 0.0f, -0.9f));
	death->SetSize(glm::vec2(1.0f, 1.0f));
	death->SetTexture("../assets/gui/Buttons/death.png");
	death->SetTransparency(1.0);
	guis[23] = death;

	score = guis[24];
	score->SetHidden(true);
	score->SetName("score");
	score->SetPosition(glm::vec3(0.0f, 0.0f, -0.9f));
	score->SetSize(glm::vec2(1.0f, 1.0f));
	score->SetTexture("../assets/gui/Buttons/score.png");
	score->SetTransparency(1.0);
	guis[24] = score;
	
}

void updateLabels() {

	guis[21]->SetSize(glm::vec2(0.2f*basehealth/maxbasehealth, 0.04f));

	sprintf(str, "%d", moneyRes);
	moneyResT->RenderText(str, 1550.0f, 20.0f, 1.5f, glm::vec3(10.0, 10.0f, 10.0f));
	
	sprintf(str, "%d", woodRes);
	woodResT->RenderText(str, 1050.0f, 20.0f, 1.5f, glm::vec3(10.0, 10.0f, 10.0f));
	
	sprintf(str, "%d", stoneRes);
	stoneResT->RenderText(str, 600.0f, 20.0f, 1.5f, glm::vec3(10.0, 10.0f, 10.0f));

	if (stoneCost > 0 || woodCost > 0 || moneyCost > 0) {
		sprintf(str, "-%d", moneyCost);
		moneyCostT->RenderText(str, 1750.0f, 20.0f, 1.5f, glm::vec3(10.0, 0.0f, 0.0f));

		sprintf(str, "-%d", woodCost);
		woodCostT->RenderText(str, 1250.0f, 20.0f, 1.5f, glm::vec3(10.0, 0.0f, 0.0f));

		sprintf(str, "-%d", stoneCost);
		stoneCostT->RenderText(str, 800.0f, 20.0f, 1.5f, glm::vec3(10.0, 0.0f, 0.0f));
	}
	else {
		sprintf(str, "", moneyCost);
		moneyCostT->RenderText(str, 1700.0f, 20.0f, 1.5f, glm::vec3(10.0, 0.0f, 0.0f));

		sprintf(str, "", woodCost);
		woodCostT->RenderText(str, 1200.0f, 20.0f, 1.5f, glm::vec3(10.0, 0.0f, 0.0f));

		sprintf(str, "", stoneCost);
		stoneCostT->RenderText(str, 700.0f, 20.0f, 1.5f, glm::vec3(10.0, 0.0f, 0.0f));
	}

	if(scoreShown){

		sprintf(str, "%d", (score1));
		p1->RenderText(str, 200.0f, 940.0f * (((float)score1)/((float)(max_score))), 1.5f, glm::vec3(10.0, 10.0f, 10.0f));

		sprintf(str, "%d", (score2));
		p2->RenderText(str, 700.0f, 940.0f * (((float)score2) / ((float)(max_score))), 1.5f, glm::vec3(10.0, 10.0f, 10.0f));

		sprintf(str, "%d", (score3));
		p3->RenderText(str, 1200.0f, 940.0f * (((float)score3) / ((float)(max_score))), 1.5f, glm::vec3(10.0, 10.0f, 10.0f));

		sprintf(str, "%d", (score4));
		p4->RenderText(str, 1700.0f, 940.0f * (((float)score4) / ((float)(max_score))), 1.5f, glm::vec3(10.0, 10.0f, 10.0f));

	}
	else {
	
		p1->RenderText("", 200.0f, 940.0f * (((float)score1) / ((float)(max_score))), 1.5f, glm::vec3(10.0, 10.0f, 10.0f));
		p2->RenderText("", 1700.0f, 940.0f * (((float)score1) / ((float)(max_score))), 1.5f, glm::vec3(10.0, 10.0f, 10.0f));
		p3->RenderText("", 1200.0f, 940.0f * (((float)score1) / ((float)(max_score))), 1.5f, glm::vec3(10.0, 10.0f, 10.0f));
		p4->RenderText("", 1500.0f, 940.0f * (((float)score1) / ((float)(max_score))), 1.5f, glm::vec3(10.0, 10.0f, 10.0f));
	}
	sprintf(str, "%d", (wavetimer));
	spawntimerT->RenderText(str, 930.0f, 940.0f, 1.2f, glm::vec3(10.0, 10.0f, 10.0f));


	sprintf(str, "Wave %d", (curWave));
	strcat(str, "/");
	sprintf(tmp_str, "%d", numWaves);
	strcat(str, tmp_str);
	waveCounter->RenderText(str, 800.0f, 1020.0f, 1.2f, glm::vec3(10.0, 10.0f, 10.0f));
	
	sprintf(str, "%d", points);
	strcat(str, " points");
	pointsT->RenderText(str, 1650.0f, 1030.0f, 0.8f, glm::vec3(10.0, 10.0f, 10.0f));
	
	sprintf(str, "%d", enemiesKilled);
	strcat(str, " kills");
	enemiesKilledT->RenderText(str, 370.0f, 1030.0f, 0.8f, glm::vec3(10.0, 10.0f, 10.0f));
	
	sprintf(str, "%d", towersBuilt);
	strcat(str, " towers");
	towersBuiltT->RenderText(str, 20.0f, 1030.0f, 0.8f, glm::vec3(10.0, 10.0f, 10.0f));
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
			entities[i]->setModelID(incomingData.models[i].modelID);
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

	//effects 
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
			//std::cout << "KILLED\n";
			int target = newCLogs.top().target;
			effect->killEffect(entities[target]->getPosition(), entities[target]->getModelID());
		}
		newCLogs.pop();
	}
	while (!newSLogs.empty()) {
		int source = newSLogs.top().source;
		int sound = newSLogs.top().sound;
		if (sound == SOUND_ID_JUMP) {
			//std::cout << "JUMPED\n";
			effect->playerJumpEffect(entities[source]->getPosition());
		}
		else if (sound == SOUND_ID_ATTACK && !newSLogs.top().stop) {
			if (entities[source]->getModelID() == MODEL_ID_TESLA || entities[source]->getModelID() == MODEL_ID_TESLA_L2 || entities[source]->getModelID() == MODEL_ID_TESLA_L3 || entities[source]->getModelID() == MODEL_ID_TESLA_L4) {
				effect->teslaAttackEffect(entities[source]->getPosition());
			}
		}
		newSLogs.pop();
	}
	effect->update(currTime);
}

void GameWorld::draw(Shader* guiShader, float wWidth, float wHeight) {

	width = wWidth;
	height = wHeight;
	float currTime = float(glfwGetTime());
	const glm::mat4& viewProjMtx = cam->GetViewProjectMtx();
	env->draw(viewProjMtx);
	std::vector<RenderEntity*> transparentEntities;
	for (RenderEntity* e : entities) {

		if (e->getActive()) {
			if (e->getModelID() == MODEL_ID_MOB_TRACTOR) {
				e->setDistFromCamera(glm::distance(cam->getCameraPosition(), e->getPosition()));
				transparentEntities.push_back(e);
			}
			else{
				e->draw(viewProjMtx, currTime, cam);
			}
		}
	}
	std::sort(transparentEntities.begin(), transparentEntities.end(), [](RenderEntity* a, RenderEntity* b)
		{
			if (a->getDistFromCamera() > b->getDistFromCamera()) return true;
			else return false;
		});

	for (HealthBar* h : healths) {
		if (h->getActive()) {
			h->draw(viewProjMtx);
		}
	}
	env->drawPortals(viewProjMtx, currTime);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (auto a : transparentEntities) {
		a->draw(viewProjMtx, currTime, cam);
	}
	glDisable(GL_BLEND);
	effect->draw(viewProjMtx, cam);
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