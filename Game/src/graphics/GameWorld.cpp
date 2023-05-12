#include "GameWorld.h"
float GameWorld::prevX, GameWorld::prevY,GameWorld::currX, GameWorld::currY, GameWorld::scrollY = 0;

void GameWorld::init() {
	currID = 0;
	env = new Skybox();

	for (int i = 0; i < NUM_PLAYERS; i++) {
		players[i] = new Player(i);
	}
	for (int i = 0; i < NUM_ENEMIES; i++) {
		mobs[i] = new Mob(i);
	}
	for (int i = 0; i < NUM_TOWERS; i++) {
		towers[i] = new Tower(i);
	}
	for (int i = 0; i < NUM_PROJECTILES; i++) {
		projectiles[i] = new Projectile(i);
	}
	for (int i = 0; i < NUM_RESOURCES; i++) {
		resources[i] = new Resource(i);
	}
	cam = new Camera();
}

void GameWorld::update(ServertoClientData& incomingData, int id) {

	unsigned int playerIndex = 0;
	unsigned int mobIndex = 0;
	unsigned int towerIndex = 0;
	unsigned int projIndex = 0;
	unsigned int resourceIndex = 0;
	for (int i = 0; i < incomingData.activity.size(); i++) {
		switch (incomingData.models[i].modelID) {
		case MODEL_ID_ROVER:
			players[i]->setActive(incomingData.activity[i]);
			if (incomingData.activity[i]) {
				players[i]->update(incomingData.positions[i], incomingData.models[i].modelOrientation);
			}
			playerIndex++;
			break;
		case MODEL_ID_MOB:
			mobs[mobIndex]->setActive(incomingData.activity[i]);
			if (incomingData.activity[i]) {
				mobs[mobIndex]->update(incomingData.positions[i], incomingData.models[i].modelOrientation);
			}
			mobIndex++;
			break;
		case MODEL_ID_TOWER:
			towers[towerIndex]->setActive(incomingData.activity[i]);
			if (incomingData.activity[i]) {
				towers[towerIndex]->update(incomingData.positions[i], incomingData.models[i].modelOrientation);
			}
			towerIndex++;
			break;
		case MODEL_ID_PROJECTILE:
			projectiles[projIndex]->setActive(incomingData.activity[i]);
			if (incomingData.activity[i]) {
				projectiles[projIndex]->update(incomingData.positions[i], incomingData.models[i].modelOrientation);
			}
			projIndex++;
			break;
		case MODEL_ID_RESOURCE:
			resources[resourceIndex]->setActive(incomingData.activity[i]);
			if (incomingData.activity[i]) {
				resources[resourceIndex]->update(incomingData.positions[i], incomingData.models[i].modelOrientation);
			}
			resourceIndex++;
			break;
		default:
			printf("UNKNOWN ENTITY ATTEMPTING TO BE RENDERED");
			break;
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