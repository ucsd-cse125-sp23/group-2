#include "GameWorld.h"
float GameWorld::prevX, GameWorld::prevY,GameWorld::currX, GameWorld::currY, GameWorld::scrollY = 0;

void GameWorld::init() {
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
	cam = new Camera();
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
void GameWorld::draw(Shader* shader, Shader* skyboxShader) {
	const glm::mat4& viewProjMtx = cam->GetViewProjectMtx();
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

}

void GameWorld::cursor_callback(GLFWwindow* window, double cX, double cY) {

	currX = cX;
	currY = cY;
}