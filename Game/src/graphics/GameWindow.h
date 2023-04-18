#pragma once
#include "Shader.h"
#include "core.h"
#include "Camera.h"
#include "Player.h"
#include "../NetworkData.h"
class GameWindow {
private:
	//Window Properties
	int width;
	int height;
	const char* windowTitle;

	//objects to render
	Player* player;

	// Shader Program
	GLuint shaderProgram;

	//camera
	Camera* cam;
public:
	GLFWwindow* window;

	GameWindow(int width, int height);
	~GameWindow();
	void setup();
	bool initializeProgram();
	bool initializeObjects();
	void cleanUp();

	void resizeCallback(GLFWwindow* window, int width, int height);

	// update and draw functions
	void idleCallback(ServertoClientData&);
	void displayCallback();
	void update(ServertoClientData &);

	// helper to reset camera
	void resetCamera();

	//callbacks - for interaction
};