#include "GameWorld.h"
float GameWorld::prevX, GameWorld::prevY,GameWorld::currX, GameWorld::currY, GameWorld::scrollY = 0;

void GameWorld::init() {

	//initialize models
	env = new Skybox();


	for (int i = 0; i < NUM_PLAYERS; i++) {
		players[i] = new Player(i);
	}
	for (int i = 0; i < NUM_ENEMIES; i++) {
		mobs[i] = new Mob(i);
	}

	cam = new Camera();
}

void GameWorld::update(ServertoClientData& incomingData, int id) {

	//currently using these indices, should probably keep track of some type of entity ID server side 
	playerIndex, mobIndex = 0;
	for (int i = 0; i < incomingData.activity.size(); i++) {
		switch (incomingData.models[i].modelID){
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
				mobs[mobIndex]->update(incomingData.positions[i]);
			}
			mobIndex++;
			break;
		default:
			break;
		}
	}


	//control thirdperson camera
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

	Player::setupMesh();
	Player::draw(viewProjMtx);

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