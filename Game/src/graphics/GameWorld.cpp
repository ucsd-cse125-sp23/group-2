#include "GameWorld.h"
float GameWorld::prevX, GameWorld::prevY, GameWorld::currX, GameWorld::currY, GameWorld::scrollY = 0;

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

	//continue
	continue_Btn = guis[0];
	continue_Btn->SetHidden(false);
	continue_Btn->SetName("continue");
	continue_Btn->SetPosition(glm::vec2(0.0f, 00.0f));
	continue_Btn->SetSize(glm::vec2(100.0f, 100.0f));
	continue_Btn->SetTexture("..assets/gui/Buttons/continue.png");
	continue_Btn->SetTransparency(0.0);
	guis[0] = continue_Btn;
	//options
	options_Btn = guis[1];
	options_Btn->SetHidden(false);
	options_Btn->SetName("options");
	options_Btn->SetPosition(glm::vec2(0.0f, 0.0f));
	options_Btn->SetSize(glm::vec2(100.0f, 100.0f));
	options_Btn->SetTexture("..assets/gui/Buttons/options");
	options_Btn->SetTransparency(0.0);
	guis[1] = options_Btn;
	//quit
	quit_Btn = guis[2];
	quit_Btn->SetHidden(false);
	quit_Btn->SetName("quit");
	quit_Btn->SetPosition(glm::vec2(0.0f, -130.0f));
	quit_Btn->SetSize(glm::vec2(100.0f, 100.0f));
	quit_Btn->SetTexture("..assets/gui/Buttons/quit");
	quit_Btn->SetTransparency(0.0);
	guis[2] = quit_Btn;

	//back
	back_Btn = guis[3];
	back_Btn->SetHidden(false	);
	back_Btn->SetName("back");
	back_Btn->SetPosition(glm::vec2(-width/2 + 50.0f, height/2 - 50.0f));
	back_Btn->SetSize(glm::vec2(100.0f, 100.0f));
	back_Btn->SetTexture("..assets/gui/Buttons/back");
	back_Btn->SetTransparency(0.0);
	guis[3] = back_Btn;

	//Volume
	volume_Sldr = guis[4];
	volume_Sldr->SetHidden(true);
	volume_Sldr->SetName("volume");
	volume_Sldr->SetPosition(glm::vec2(0.0f, 0.0f));
	volume_Sldr->SetSize(glm::vec2(100.0f, 100.0f));
	volume_Sldr->SetTexture("..assets/gui/Buttons/volume");
	volume_Sldr->SetTransparency(0.0);
	guis[4] = volume_Sldr;
	volume_Sldr_Back = guis[5];
	volume_Sldr_Back->SetHidden(true);
	volume_Sldr_Back->SetName("volume_Sldr_Back");
	volume_Sldr_Back->SetPosition(glm::vec2(0.0f, 0.0f));
	volume_Sldr_Back->SetSize(glm::vec2(100.0f, 100.0f));
	volume_Sldr_Back->SetTexture("..assets/gui/Buttons/volume_Sldr_Back");
	volume_Sldr_Back->SetTransparency(0.0);
	guis[5] = volume_Sldr_Back;
	volume_Sldr_Front = guis[6];
	volume_Sldr_Front->SetHidden(true);
	volume_Sldr_Front->SetName("volume_Sldr_Front");
	volume_Sldr_Front->SetPosition(glm::vec2(0.0f, 0.0f));
	volume_Sldr_Front->SetSize(glm::vec2(100.0f, 100.0f));
	volume_Sldr_Front->SetTexture("..assets/gui/Buttons/volume_Sldr_Front");
	volume_Sldr_Front->SetTransparency(0.0);
	guis[6] = volume_Sldr_Front;

	//Health
	health_Bar = guis[7];
	health_Bar->SetHidden(true);
	health_Bar->SetName("health");
	health_Bar->SetPosition(glm::vec2(0.0f, 0.0f));
	health_Bar->SetSize(glm::vec2(100.0f, 100.0f));
	health_Bar->SetTexture("..assets/gui/Buttons/health");
	health_Bar->SetTransparency(0.0);
	guis[7] = health_Bar;
	health_Bar_Back = guis[8];
	health_Bar_Back->SetHidden(true);
	health_Bar_Back->SetName("health_back");
	health_Bar_Back->SetPosition(glm::vec2(0.0f, 0.0f));
	health_Bar_Back->SetSize(glm::vec2(100.0f, 100.0f));
	health_Bar_Back->SetTexture("..assets/gui/Buttons/health_back");
	health_Bar_Back->SetTransparency(0.0);
	guis[8] = health_Bar_Back;
	health_Bar_Front = guis[9];
	health_Bar_Front->SetHidden(true);
	health_Bar_Front->SetName("health_front");
	health_Bar_Front->SetPosition(glm::vec2(0.0f, 0.0f));
	health_Bar_Front->SetSize(glm::vec2(100.0f, 100.0f));
	health_Bar_Front->SetTexture("..assets/gui/Buttons/health_front");
	health_Bar_Front->SetTransparency(0.0);
	guis[9] = health_Bar_Front;

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

	currX = cX;
	currY = cY;
}