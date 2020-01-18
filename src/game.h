#pragma once

#include <thread>

// all non-graphics, logic stuff should go in here.
// contains the game loop.
// creates and returns a new thread
std::thread* startGame(GLFWwindow* window);