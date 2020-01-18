#pragma once

#include <thread>

namespace Game {
	// need to expose these so they can be set in main()
	void mousePositionCallback(GLFWwindow* window, double x, double y);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
}

// all non-graphics, logic stuff should go in here.
// contains the game loop.
// creates and returns a new thread
std::thread* startGame(GLFWwindow* window);