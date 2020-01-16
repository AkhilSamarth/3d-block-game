#include <iostream>

#include "chunk.h"

Chunk::Chunk(glm::vec2 pos) : pos(glm::vec3(pos.x, 0, pos.y)), blocks(), neighborChunks() {}

void Chunk::generateBlocks() {}

void Chunk::updateBlockFaces() {
	// loop through all inner (non-boundary blocks)
	for (int x = 1; x < CHUNK_SIZE - 1; x++) {
		for (int z = 1; z < CHUNK_SIZE - 1; z++) {
			for (int y = 0; y < WORLD_HEIGHT; y++) {

			}
		}
	}
}

void Chunk::addNeighbor(Chunk* chunk) {
	// get the position of the neighbor chunk
	glm::vec3 chunkPos = chunk->getPosition();

	// make sure the neighbor is actually next to this chunk
	// must be either same x/diff z or same z/diff x
	if ((chunkPos.x == pos.x && chunkPos.z == pos.z) || (chunkPos.x != pos.x && chunkPos.z != pos.z)) {
		std::cerr << "Incorrect neighbor chunk given!" << std::endl;
		return;
	}

	// figure our which chunk this is and add it to the right spot
	if (chunkPos.z < pos.z) {
		// front
		neighborChunks[0] = chunk;
	}
	else if (chunkPos.x > pos.x) {
		// right
		neighborChunks[1] = chunk;
	}
	else if (chunkPos.z > pos.z) {
		// back
		neighborChunks[2] = chunk;
	}
	else {
		// left
		neighborChunks[3] = chunk;
	}
}