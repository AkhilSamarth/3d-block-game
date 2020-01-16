#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "block.h"

#define CHUNK_SIZE 8	// each chunk will be a column with this length and width

class Chunk {
private:
	std::vector<Block> blocks;	// list of all the blocks in this chunk
	glm::vec3 pos;		// position of back, left, bottom corner (lowest x, y, z) along integer grid (must be multiple of CHUNK_SIZE)
public:
	Chunk(glm::vec3 pos);	// create a chunk at this position

	void generateBlocks();		// fill blocks
};