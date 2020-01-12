#include <iostream>
#include <thread>
#include <chrono>

#include <GLFW\glfw3.h>

#include "camera.h"

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
	using namespace std::chrono;	// chrono being used for timekeeping to limit loop

	// how many ticks per second the game should run at (iterations of game loop per second)
	static const double TPS = 100;
	static const uint64_t MICROS_PER_TICK = 1000000 / TPS;		// the number of microseconds between each tick
	static uint64_t lastTime = (duration_cast<microseconds>(steady_clock::now().time_since_epoch())).count();

	// mouse input setup
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);	// add mouse callback

	// keep running until window should close (same as rendering loop)
	while (!glfwWindowShouldClose(window)) {
		lastTime = (duration_cast<microseconds>(steady_clock::now().time_since_epoch())).count();		// update lastTime
		
		processKeys(window);

		// make sure loop stays below TPS
		uint64_t currentTime = (duration_cast<microseconds>(steady_clock::now().time_since_epoch())).count();
		uint64_t delta = currentTime - lastTime;	// time taken for loop to complete
		if (delta < MICROS_PER_TICK) {
			uint64_t timeRemaining = MICROS_PER_TICK - delta;		// time until next loop should run
			std::this_thread::sleep_for(microseconds(timeRemaining));
		}
	}
}