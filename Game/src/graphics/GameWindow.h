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

	void resizeCallback(GLFWwindow* window, int width, int height);

	// update and draw functions
	void idleCallback(ServertoClientData&, int);
	void displayCallback();
	void update(ServertoClientData &, int);

	// helper to reset camera
	void resetCamera();

	//callbacks - for interaction
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double currX, double currY);
};