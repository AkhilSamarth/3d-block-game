#include <iostream>
#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "game.h"
#include "camera.h"
#include "chunk.h"

#define MOUSE_SENS 0.08		// mouse sensitivity
#define MOVE_SPEED 5		// speed on key presses (units per second)

namespace Game {
	// used to process mouse movement
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

	// update chunks, either all or by neighbor
	// if by neighbor, starts with chunk the camera is in
	// doneUpdating changes to true when chunk updating is finished
	std::thread* loadChunks(bool byNeighbor, bool& doneUpdating) {
		static bool running = false;	// make sure only one chunk loader is running at a time

		if (running) {
			std::cerr << "Chunk loader is already running." << std::endl;
			return nullptr;
		}

		running = true;
		doneUpdating = false;

		if (byNeighbor) {
			// get chunk based on cam position
			glm::vec3 camPos = Camera::getActiveCam()->getPosition();
			int chunkX, chunkZ;
			Chunk::getChunkPosition(camPos.x, camPos.z, chunkX, chunkZ);
			uint32_t chunkIndex = Chunk::getChunkIndex(chunkX, chunkZ);
			if (Chunk::chunkList.find(chunkIndex) == Chunk::chunkList.end()) {
				std::cerr << "Active cam is in non-existant chunk. Updating all chunks." << std::endl;
				// update all chunks instead
				return new std::thread(Chunk::updateAllChunks, doneUpdating);
			}
			Chunk* starter = Chunk::chunkList[chunkIndex];

			return new std::thread(Chunk::updateChunksByNeighbor, starter, doneUpdating);
		}
		else {
			return new std::thread(Chunk::updateAllChunks, doneUpdating);
		}

		running = false;
	}

	std::thread* loadSingleChunk() {}

	void startGame(GLFWwindow* window) {
		// mouse input setup
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouseCallback);

		float delta = glfwGetTime();		// used to keep track of time between loop iterations

		bool chunkLoaderDone = false;	// used to join chunk loading threads once loading finishes
		std::thread* chunkLoader;		// keep track of the chunk loading thread

		// initial chunk load
		chunkLoader = Game::loadChunks(true, chunkLoaderDone);

		// keep running until window should close (same as rendering loop)
		while (!glfwWindowShouldClose(window)) {
			float loopStartTime = glfwGetTime();	// used to update delta
			processKeys(window, delta);

			// join and delete chunkLoader thread if needed
			if (chunkLoaderDone) {
				chunkLoader->join();
				delete chunkLoader;
				chunkLoaderDone = false;
			}

			delta = glfwGetTime() - loopStartTime;
		}
	}
}

std::thread* startGame(GLFWwindow* window) {
	static bool gameStarted = false;	// make sure this only runs once

	if (gameStarted) {
		std::cerr << "Attempted to start a second game thread." << std::endl;
		return nullptr;
	}

	gameStarted = true;
	return new std::thread(Game::startGame, window);
}