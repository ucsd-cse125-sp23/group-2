#include "GameWorld.h"

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
}

void GameWorld::update(ServertoClientData& incomingData, int id) {
	for (int i = 0; i < NUM_PLAYERS; i++) {
		players[i]->setActive(incomingData.activity[i]);
		if (incomingData.activity[i]) {
			players[i]->update(incomingData.positions[i]);
		}
	}

	for (int i = ENEMY_START; i < ENEMY_END; i++) {
		mobs[i - ENEMY_START]->setActive(incomingData.activity[i]);
		if (incomingData.activity[i]) {
			mobs[i - ENEMY_START]->update(incomingData.positions[i]);
		}
	}
}

//render all active entities
void GameWorld::draw(const glm::mat4& viewProjMtx, Shader* shader, Shader* skyboxShader) {
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