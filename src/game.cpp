#include <iostream>
#include <queue>
#include <set>
#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "game.h"
#include "camera.h"
#include "chunk.h"
#include "terrain.h"

#define MOUSE_SENS 0.08		// mouse sensitivity
#define MOVE_SPEED 10		// speed on key presses (units per second)

#define TRACE_RANGE 4.0f	// how far the player can reach
#define TRACE_STEP 0.01f	// interval at which block position is calculated (lower = more accurate, less performance)

#define TERRAIN_DISTANCE 8	// how many chunks from camera to generate (1 = neighbors, 2 = neighbor's neighbors, etc.)

namespace Game {

	std::queue<uint32_t> chunksToUpdate;	// a queue of chunks (described by index) to update

	// adds this chunk to the chunk updater's queue
	void updateChunk(int x, int z) {
		// make sure chunk exists
		int chunkX, chunkZ;
		Chunk::getChunkPosition(x, z, chunkX, chunkZ);
		int chunkIndex = Chunk::getChunkIndex(chunkX, chunkZ);
		if (Chunk::chunkList.find(chunkIndex) == Chunk::chunkList.end()) {
			return;
		}

		// add to queue
		chunksToUpdate.push(chunkIndex);
	}

	// update chunk function for immediate chunk updates (done in this thread)
	// warning: does not guarantee update (i.e. if updateData is locked by another thread)
	// if update fails, adds to queue
	void updateChunkQuickly(int x, int z) {
		// make sure chunk exists
		int chunkX, chunkZ;
		Chunk::getChunkPosition(x, z, chunkX, chunkZ);
		int chunkIndex = Chunk::getChunkIndex(chunkX, chunkZ);
		if (Chunk::chunkList.find(chunkIndex) == Chunk::chunkList.end()) {
			return;
		}

		// attempt update
		if (!Chunk::chunkList[chunkIndex]->updateData()) {
			// add to queue
			chunksToUpdate.push(chunkIndex);
		}
	}

	// update chunks based on block
	// updates neighbor chunks for edge and corner blocks
	// doesn't create new thread
	void updateBlock(int x, int z) {
		// update current chunk
		updateChunkQuickly(x, z);

		// calculate local block position
		int chunkX, chunkZ;
		Chunk::getChunkPosition(x, z, chunkX, chunkZ);
		int localX = x - chunkX;
		int localZ = z - chunkZ;
		
		// check if neighbors need updating
		// check x
		if (localX == 0) {
			// left edge
			updateChunkQuickly(x - 1, z);
		}
		else if (localX == CHUNK_SIZE - 1) {
			// right edge
			updateChunkQuickly(x + 1, z);
		}
		// check z
		if (localZ == 0) {
			// front edge
			updateChunkQuickly(x, z - 1);
		}
		else if (localZ == CHUNK_SIZE - 1) {
			// back edge
			updateChunkQuickly(x, z + 1);
		}
	}

	// helper for updateChunks()
	void updateChunksHelper(GLFWwindow* window) {
		// loop while window is open
		while (!glfwWindowShouldClose(window)) {
			// loop through queue and update
			while (!chunksToUpdate.empty()) {
				// get chunk index from queue
				uint32_t index = chunksToUpdate.front();
				chunksToUpdate.pop();

				// make sure chunk exists
				if (Chunk::chunkList.find(index) == Chunk::chunkList.end()) {
					continue;
				}

				// attempt to update chunk
				if (!Chunk::chunkList[index]->updateData()) {
					// if update was unsuccessful, send to back of queue to try again
					chunksToUpdate.push(index);
				}
			}
		}
	}

	// creates a thread continuously loops through chunks queue and updates
	std::thread* updateChunks(GLFWwindow* window) {
		return new std::thread(updateChunksHelper, window);
	}

	// generate the chunk at (x, z) if it doesn't already exist
	// returns whether or not a chunk was generated
	bool genChunk(int x, int z) {
		// make sure chunk doesn't already exist
		uint32_t chunkIndex = Chunk::getChunkIndex(x, z);
		if (Chunk::chunkList.find(chunkIndex) != Chunk::chunkList.end()) {
			return false;
		}

		// create chunk
		Chunk* chunk = new Chunk(glm::ivec2(x, z));
		chunk->generateBlocks(Terrain::flat10);

		return true;
	}

	// genTerrain calls this function in its thread
	void genTerrainHelper(GLFWwindow* window) {
		// loop while window is open
		while (!glfwWindowShouldClose(window)) {
			glm::vec3 camPos = Camera::getActiveCam()->getActiveCam()->getPosition();
			int chunkX, chunkZ;
			Chunk::getChunkPosition(camPos.x, camPos.z, chunkX, chunkZ);	// initially set to camera's chunk

			// queue and set to keep track of outer chunks for each iteration, store chunk index
			std::queue<uint32_t> chunksToGo;
			std::set<uint32_t> chunksDone;

			// add camera chunk to queue
			chunksToGo.push(Chunk::getChunkIndex(chunkX, chunkZ));
			chunksDone.insert(Chunk::getChunkIndex(chunkX, chunkZ));

			for (int i = 0; i <= TERRAIN_DISTANCE; i++) {
				// process everything currently in the queue
				int queueSize = chunksToGo.size();
				for (int j = 0; j < queueSize; j++) {
					// generate current chunk
					glm::ivec2 current = Chunk::getChunkPositionFromIndex(chunksToGo.front());
					chunksToGo.pop();
					// if this chunk was generated, update it and any neighbors it has
					if (genChunk(current.x, current.y)) {
						updateChunk(current.x, current.y);

						updateChunk(current.x, current.y - CHUNK_SIZE);	// front
						updateChunk(current.x + CHUNK_SIZE, current.y);	// right
						updateChunk(current.x, current.y + CHUNK_SIZE);	// back
						updateChunk(current.x - CHUNK_SIZE, current.y);	// left
					}

					// add neighbors to queue if they haven't been processed
					// right
					if (chunksDone.find(Chunk::getChunkIndex(current + glm::ivec2(CHUNK_SIZE, 0))) == chunksDone.end()) {
						chunksToGo.push(Chunk::getChunkIndex(current + glm::ivec2(CHUNK_SIZE, 0)));
						chunksDone.insert(Chunk::getChunkIndex(current + glm::ivec2(CHUNK_SIZE, 0)));
					}
					// left
					if (chunksDone.find(Chunk::getChunkIndex(current - glm::ivec2(CHUNK_SIZE, 0))) == chunksDone.end()) {
						chunksToGo.push(Chunk::getChunkIndex(current - glm::ivec2(CHUNK_SIZE, 0)));
						chunksDone.insert(Chunk::getChunkIndex(current - glm::ivec2(CHUNK_SIZE, 0)));
					}
					// front
					if (chunksDone.find(Chunk::getChunkIndex(current - glm::ivec2(0, CHUNK_SIZE))) == chunksDone.end()) {
						chunksToGo.push(Chunk::getChunkIndex(current - glm::ivec2(0, CHUNK_SIZE)));
						chunksDone.insert(Chunk::getChunkIndex(current - glm::ivec2(0, CHUNK_SIZE)));
					}
					// back
					if (chunksDone.find(Chunk::getChunkIndex(current + glm::ivec2(0, CHUNK_SIZE))) == chunksDone.end()) {
						chunksToGo.push(Chunk::getChunkIndex(current + glm::ivec2(0, CHUNK_SIZE)));
						chunksDone.insert(Chunk::getChunkIndex(current + glm::ivec2(0, CHUNK_SIZE)));
					}
				}
			}
		}
	}

	// creates a thread to keep generating chunks within the given distance of the active cam
	std::thread* genTerrain(GLFWwindow* window) {
		return new std::thread(genTerrainHelper, window);
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

		Camera::getActiveCam()->translate(glm::vec3(0, 15, 0));

		// start threads for terrain gen and chunk updating
		std::thread* terrainGenThread = genTerrain(window);
		std::thread* chunkUpdateThread = updateChunks(window);

		// keep running until window should close (same as rendering loop)
		while (!glfwWindowShouldClose(window)) {
			float loopStartTime = glfwGetTime();	// used to update delta
			processKeys(window, delta);

			delta = glfwGetTime() - loopStartTime;
		}

		// kill terrain gen thread
		terrainGenThread->join();
		delete terrainGenThread;

		// kill chunk updater
		chunkUpdateThread->join();
		delete chunkUpdateThread;
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