	#include "GameWorld.h"
	float GameWorld::prevX, GameWorld::prevY, GameWorld::currX, GameWorld::currY, GameWorld::scrollY = 0;
	std::array <GUIElement*, NUM_GUI> guis;
	int selected;
	int depth;
	bool menuOn;
	void GameWorld::init() {

	//UI
	//Buttons
	currID = 0;
	env = new Skybox();

	for (int i = 0; i < NUM_PLAYERS; i++) {
		players[i] = new Player(currID);
		currID++;
	}
	for (int i = ENEMY_START; i < ENEMY_END; i++) {
		mobs[i - ENEMY_START] = new Mob(currID);
		currID++;
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
	continue_Btn->SetPosition(glm::vec2(0.0f, 0.30f));
	continue_Btn->SetSize(glm::vec2(0.75f, 0.25f));
	continue_Btn->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/continueH.jpg");
	continue_Btn->SetTransparency(1.0);
	guis[0] = continue_Btn;
	//options
	options_Btn = guis[1];
	options_Btn->SetHidden(true);
	options_Btn->SetName("options");
	options_Btn->SetPosition(glm::vec2(0.0f, 0.0f));
	options_Btn->SetSize(glm::vec2(0.75f, 0.25f));
	options_Btn->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/options.jpg");
	options_Btn->SetTransparency(1.0);
	guis[1] = options_Btn;
	//quit
	quit_Btn = guis[2];
	quit_Btn->SetHidden(true);
	quit_Btn->SetName("quit");
	quit_Btn->SetPosition(glm::vec2(0.0f, -0.30f));
	quit_Btn->SetSize(glm::vec2(0.75f, 0.25f));
	quit_Btn->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/quit.jpg");
	quit_Btn->SetTransparency(1.0);
	guis[2] = quit_Btn;

	//back
	back_Btn = guis[3];
	back_Btn->SetHidden(true);
	back_Btn->SetName("back");
	back_Btn->SetPosition(glm::vec2(0.0f, -0.3f));
	back_Btn->SetSize(glm::vec2(0.75f, 0.25f));
	back_Btn->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/back.jpg");
	back_Btn->SetTransparency(1.0);
	guis[3] = back_Btn;

	//Volume
	volume_Sldr = guis[4];
	volume_Sldr->SetHidden(true);
	volume_Sldr->SetName("volume");
	volume_Sldr->SetPosition(glm::vec2(-0.5f, 0.0f));
	volume_Sldr->SetSize(glm::vec2(0.75f, 0.25f));
	volume_Sldr->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/volume.jpg");
	volume_Sldr->SetTransparency(1.0);
	guis[4] = volume_Sldr;
	volume_Sldr_Back = guis[5];
	volume_Sldr_Back->SetHidden(true);
	volume_Sldr_Back->SetName("volume_Sldr_Back");
	volume_Sldr_Back->SetPosition(glm::vec2(0.5f, 0.0f));
	volume_Sldr_Back->SetSize(glm::vec2(0.75f, 0.1f));
	volume_Sldr_Back->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/volume_Sldr_Back.jpg");
	volume_Sldr_Back->SetTransparency(0.0);
	guis[5] = volume_Sldr_Back;
	volume_Sldr_Front = guis[6];
	volume_Sldr_Front->SetHidden(true);
	volume_Sldr_Front->SetName("volume_Sldr_Front");
	volume_Sldr_Front->SetPosition(glm::vec2(0.5f, 0.0f));
	volume_Sldr_Front->SetSize(glm::vec2(0.75f, 0.1f));
	volume_Sldr_Front->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/volume_Sldr_Front.jpg");
	volume_Sldr_Front->SetTransparency(1.0);
	guis[6] = volume_Sldr_Front;

	//Health
	health_Bar = guis[7];
	health_Bar->SetHidden(true);
	health_Bar->SetName("health");
	health_Bar->SetPosition(glm::vec2(0.0f, 0.0f));
	health_Bar->SetSize(glm::vec2(1.0f, 1.0f));
	health_Bar->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/health.jpg");
	health_Bar->SetTransparency(1.0);
	guis[7] = health_Bar;
	health_Bar_Back = guis[7];
	health_Bar_Back->SetHidden(true);
	health_Bar_Back->SetName("health_back");
	health_Bar_Back->SetPosition(glm::vec2(0.0f, 0.0f));
	health_Bar_Back->SetSize(glm::vec2(1.0f, 1.0f));
	health_Bar_Back->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/health_back.jpg");
	health_Bar_Back->SetTransparency(1.0);
	guis[8] = health_Bar_Back;
	health_Bar_Front = guis[9];
	health_Bar_Front->SetHidden(true);
	health_Bar_Front->SetName("health_front");
	health_Bar_Front->SetPosition(glm::vec2(0.0f, 0.0f));
	health_Bar_Front->SetSize(glm::vec2(1.0f, 1.0f));
	health_Bar_Front->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/health_front.jpg");
	health_Bar_Front->SetTransparency(1.0);
	guis[9] = health_Bar_Front;

	blank = guis[10];
	blank->SetHidden(true);
	blank->SetName("health_front");
	blank->SetPosition(glm::vec2(0.5f, 0.0f));
	blank->SetSize(glm::vec2(0.75f, 0.25f));
	blank->SetTexture("C:/Users/V/Downloads/G/Game/assets/gui/Buttons/blank.jpg");
	blank->SetTransparency(1.0);
	guis[10] = blank;

}

void GameWorld::update(ServertoClientData& incomingData, int id) {
	for (int i = 0; i < NUM_PLAYERS; i++) {
		players[i]->setActive(incomingData.activity[i]);
		if (incomingData.activity[i]) {
			players[i]->update(incomingData.positions[i], incomingData.models[i].modelOrientation);
		}
	}

	for (int i = ENEMY_START; i < ENEMY_END; i++) {
		mobs[i - ENEMY_START]->setActive(incomingData.activity[i]);
		if (incomingData.activity[i]) {
			mobs[i - ENEMY_START]->update(incomingData.positions[i]);
		}
	}

	int maxDelta = 100;
	int dx = glm::clamp((int)(currX - prevX), -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)(currY - prevY)), -maxDelta, maxDelta);
	prevX = (int)currX;
	prevY = (int)currY;
	cam->update(players[id]->getPosition(), dx, dy, scrollY);
}

//render all active entities
void GameWorld::draw(Shader* shader, Shader* skyboxShader, Shader* guiShader, float wWidth, float wHeight) {
	const glm::mat4& viewProjMtx = cam->GetViewProjectMtx();

	width = wWidth;
	height = wHeight;
	env->draw(viewProjMtx, skyboxShader);

	for (Player* p : players) {
		if (p->getActive()) {
			p->draw(viewProjMtx, shader);
		}
	}

	for (Mob* m : mobs) {
		if (m->getActive()) {
			m->draw(viewProjMtx, shader);
		}
	}

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