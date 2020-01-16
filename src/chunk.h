#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "block.h"

#define CHUNK_SIZE 8		// each chunk will be a column with this length and width
#define WORLD_HEIGHT 32		// height of the world 

class Chunk {
private:
	Block* blocks[CHUNK_SIZE][WORLD_HEIGHT][CHUNK_SIZE];	// pointers to all blocks in this chunk at correct position
	Chunk* neighborChunks[4];		// pointers to surrounding chunks in order (front, right, back, left)
	glm::vec3 pos;		// position of left, bottom, front corner (lowest x, y, z) along integer grid (must be multiple of CHUNK_SIZE)
public:
	Chunk(glm::vec3 pos);	// create a chunk at this position

	void generateBlocks();		// fill blocks
	void updateBlockFaces();	// set which faces of each block are exposed

	void addNeighbor(Chunk* chunk);		// add a neighboring chunk
};