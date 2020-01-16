#pragma once

#include <map>

#include <glm/glm.hpp>

#include "block.h"
#include "drawing.h"

#define CHUNK_SIZE 8		// each chunk will be a column with this length and width
#define WORLD_HEIGHT 32		// height of the world 

class Chunk {
private:
	static std::map<glm::ivec2, Chunk*> chunkList;		// a list of all the chunks mapped using chunk (x, z) position

	Block* blocks[CHUNK_SIZE][WORLD_HEIGHT][CHUNK_SIZE];	// pointers to all blocks in this chunk at correct position
	Chunk* neighborChunks[4];		// pointers to surrounding chunks in order (front, right, back, left)
	glm::ivec3 pos;		// position of left, front corner (lowest x, z, y always 0) along integer grid (must be multiple of CHUNK_SIZE)
	std::vector<Vertex> verts;	// all vertices of all faces which should be drawn of blocks in this chunk
	bool updated;		// whether or not the block faces and verts of this chunk are up-to-date
	
	void updateBlockFaces();	// set which faces of each block are exposed
	void updateVerts();		// update the verts vector with the correct vertices
public:
	static void addBlock(Block* block, int x, int y, int z);	// add block to correct chunk at position (x, y, z) in global coords
	static Block* removeBlock(int x, int y, int z);	// remove and return the block at (x, y, z) in global coords

	Chunk(glm::ivec2 pos);	// create a chunk at the given (x, z)
	~Chunk();
	glm::ivec3 getPosition();	// returns the position of this chunk
	void update();		// update the block faces and vertices of this chunk
	bool isUpdated();	// whether or not the face/vertex data of this chunk is up to date
	std::vector<Vertex> getVertices();		// return the vertices array
	void addNeighbor(Chunk* chunk);		// add a neighboring chunk
};