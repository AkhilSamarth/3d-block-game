#include <iostream>

#include "chunk.h"

Chunk::Chunk(glm::vec2 pos) : pos(glm::vec3(pos.x, 0, pos.y)), blocks(), neighborChunks() {}

void Chunk::generateBlocks() {}

void Chunk::updateBlockFaces() {
	// loop through all inner (non-boundary blocks)
	for (int x = 1; x < CHUNK_SIZE - 1; x++) {
		for (int z = 1; z < CHUNK_SIZE - 1; z++) {
			for (int y = 0; y < WORLD_HEIGHT; y++) {
				// set face status of neighbor blocks depending on this one
				// if this block exists, neighbor blocks have hidden faces
				// if it doesn't exist, neighbor blocks have exposed faces
				bool exposed = (blocks[x][y][z] == nullptr);
				
				// set status of neighbor blocks, if they exist
				Block* neighbor;
				if ((neighbor = blocks[x + 1][y][z]) != nullptr) {
					neighbor->boolFace(BIT_FACE_LEFT, exposed);
				}
				else if ((neighbor = blocks[x - 1][y][z]) != nullptr) {
					neighbor->boolFace(BIT_FACE_RIGHT, exposed);
				}
				else if ((neighbor = blocks[x][y + 1][z]) != nullptr) {
					neighbor->boolFace(BIT_FACE_BOTTOM, exposed);
				}
				else if ((neighbor = blocks[x][y - 1][z]) != nullptr) {
					neighbor->boolFace(BIT_FACE_TOP, exposed);
				}
				else if ((neighbor = blocks[x][y][z + 1]) != nullptr) {
					neighbor->boolFace(BIT_FACE_FRONT, exposed);
				}
				else if ((neighbor = blocks[x][y][z - 1]) != nullptr) {
					neighbor->boolFace(BIT_FACE_BACK, exposed);
				}
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