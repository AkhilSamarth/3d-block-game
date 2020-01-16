#include <iostream>
#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "camera.h"
#include "chunk.h"

void mouseCallback(GLFWwindow* window, double x, double y) {
	static const float sensitivity = 0.08;

	// need to keep track of previous x and y to calculate deltas
	static double lastX = x;
	static double lastY = y;

	// calculate deltas
	double deltaX = x - lastX;
	double deltaY = y - lastY;

	// rotate camera
	Camera::getActiveCam()->rotateYaw(-deltaX * sensitivity);
	Camera::getActiveCam()->rotatePitch(-deltaY * sensitivity);

	// update "last" vars
	lastX = x;
	lastY = y;
}

void processKeys(GLFWwindow* window) {
	// how fast camera should move when a key is pressed
	static const float camSpeed = 0.05;

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

	// keep running until window should close (same as rendering loop)
	while (!glfwWindowShouldClose(window)) {
		processKeys(window);
	}
}