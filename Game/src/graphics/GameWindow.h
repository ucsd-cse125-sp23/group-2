#pragma once
#include "core.h"
#include "Camera.h"
#include "GameWorld.h"
#include "../NetworkData.h"
class GameWindow {
private:
	//Window Properties
	int width;
	int height;
	float lastTime;
	const char* windowTitle;

	//objects to render
	GameWorld* gameWorld;

	// Shader Program
	Shader* shaderProgram;
	Shader* skyboxProgram;

	
	//camera
	//Camera* cam;
	//static float prevX, prevY, scrollY;
	//static int mouseDX, mouseDY;
public:
	GLFWwindow* window;

	GameWindow(int width, int height);
	~GameWindow();
	void setup();
	bool initializeProgram();
	bool initializeObjects();
	void cleanUp();


	// update and draw functions
	void idleCallback(ServertoClientData&, int);
	void displayCallback();
	void update(ServertoClientData &, int);

	// helper to reset camera
	void resetCamera();

	float getCamAngle() { return gameWorld->getCamAngle(); };
	glm::vec3 getCamDirectionVector() { return gameWorld->getCamDirectionVector(); };
	glm::vec3 getCamUpVector() { return gameWorld->getCamUpVector(); };
	glm::vec3 getCamPosition() { return gameWorld->getCamPosition(); };

	//callbacks - for interaction
	static void resizeCallback(GLFWwindow* window, int width, int height);
};