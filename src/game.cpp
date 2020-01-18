#include <iostream>
#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "game.h"
#include "camera.h"
#include "chunk.h"

#define MOUSE_SENS 0.08		// mouse sensitivity
#define MOVE_SPEED 5		// speed on key presses (units per second)

#define TRACE_RANGE 4.0f	// how far the player can reach
#define TRACE_STEP 0.01f	// interval at which block position is calculated (lower = more accurate, less performance)

namespace Game {
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

	// update single chunk, the one containing global position (x, ~ ,z)
	// doesn't create a separate thread
	void updateChunk(int x, int z) {
		printf("updating chunk containing: %d, %d\n", x, z);

		int chunkX, chunkZ;
		Chunk::getChunkPosition(x, z, chunkX, chunkZ);
		int chunkIndex = Chunk::getChunkIndex(chunkX, chunkZ);
		if (Chunk::chunkList.find(chunkIndex) == Chunk::chunkList.end()) {
			return;
		}

		// update chunk
		Chunk::chunkList[chunkIndex]->updateData();
	}

	// update chunks based on block
	// same as updateChunk, but also updates neighbor chunks for edge and corner blocks
	void updateBlock(int x, int z) {
		printf("block: %d, %d\n", x, z);
		// update current chunk
		updateChunk(x, z);

		// calculate local block position
		int chunkX, chunkZ;
		Chunk::getChunkPosition(x, z, chunkX, chunkZ);
		int localX = x - chunkX;
		int localZ = z - chunkZ;
		
		// check if neighbors need updating
		// check x
		if (localX == 0) {
			printf("left\n");
			// left edge
			updateChunk(x - 1, z);
		}
		else if (localX == CHUNK_SIZE - 1) {
			printf("right\n");
			// right edge
			updateChunk(x + 1, z);
		}
		// check z
		if (localZ == 0) {
			printf("front\n");
			// front edge
			updateChunk(x, z - 1);
		}
		else if (localZ == CHUNK_SIZE - 1) {
			printf("back\n");
			// back edge
			updateChunk(x, z + 1);
		}
	}

	// mouse movement
	void mousePositionCallback(GLFWwindow* window, double x, double y) {
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

	// call function on left click
	void leftClick() {
		// left button press
		glm::vec3 camPos = Camera::getActiveCam()->getPosition();		// camera position
		glm::vec3 step = TRACE_STEP * Camera::getActiveCam()->getForward();	// scaled down version of forward vector
		glm::vec3 tracePos = camPos;		// position the trace is currently checking
		glm::ivec3 lastBlock = Block::getBlockPositionFromGlobal(camPos);	// ignore block cam is currently in and any blocks that have already been checked
		glm::ivec3 traceBlock;	// block the trace is in

		static const int ITERATIONS = TRACE_RANGE / TRACE_STEP;	// number of iterations of trace loop to run

		for (int i = 0; i < ITERATIONS; i++) {
			tracePos += step;	// update trace position

			traceBlock = Block::getBlockPositionFromGlobal(tracePos);	// block the trace is currently in

			// if this is the camera's block, ignore it
			if (traceBlock == lastBlock) {
				continue;
			}

			// check if the block exists, 
			if (Chunk::checkBlock(traceBlock.x, traceBlock.y, traceBlock.z)) {
				// remove block, update chunks, and stop
				Chunk::removeBlock(traceBlock.x, traceBlock.y, traceBlock.z);
				updateBlock(traceBlock.x, traceBlock.z);
				return;
			}

			// update lastBlock
			lastBlock = traceBlock;
		}
	}

	// mouse clicks
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1) {
			leftClick();
		}
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
		float delta = glfwGetTime();		// used to keep track of time between loop iterations

		bool chunkLoaderDone = false;	// used to join chunk loading threads once loading finishes
		std::thread* chunkLoader = nullptr;		// keep track of the chunk loading thread

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