#include <iostream>
#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "camera.h"
#include "chunk.h"

#define MOUSE_SENS 0.08		// mouse sensitivity
#define MOVE_SPEED 5		// speed on key presses (units per second)

void mouseCallback(GLFWwindow* window, double x, double y) {
	// need to keep track of previous x and y to calculate deltas
	static double lastX = x;
	static double lastY = y;

	// calculate deltas
	double deltaX = x - lastX;
	double deltaY = y - lastY;

	// rotate camera
	Camera::getActiveCam()->rotateYaw(-deltaX * MOUSE_SENS);
	Camera::getActiveCam()->rotatePitch(-deltaY * MOUSE_SENS);

	// update "last" vars
	lastX = x;
	lastY = y;
}

// deals with key presses
// delta is used to make sure movement speed doesn't change based on computer performance
void processKeys(GLFWwindow* window, float delta) {
	float camSpeed = MOVE_SPEED * delta;

	// process key presses
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Camera::getActiveCam()->translate(Camera::getActiveCam()->getForward() * camSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Camera::getActiveCam()->translate(Camera::getActiveCam()->getForward() * -camSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		Camera::getActiveCam()->translate(Camera::getActiveCam()->getRight() * -camSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		Camera::getActiveCam()->translate(Camera::getActiveCam()->getRight() * camSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		Camera::getActiveCam()->translate(Camera::getActiveCam()->getUp() * camSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		Camera::getActiveCam()->translate(Camera::getActiveCam()->getUp() * -camSpeed);
	}
}

void startGame(GLFWwindow* window) {
	// mouse input setup
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);	// add mouse callback

	float delta = glfwGetTime();		// used to keep track of time between loop iterations
	// keep running until window should close (same as rendering loop)
	while (!glfwWindowShouldClose(window)) {
		float loopStartTime = glfwGetTime();	// used to update delta

		processKeys(window, delta);
	
		delta = glfwGetTime() - loopStartTime;
	}
}