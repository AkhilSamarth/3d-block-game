#pragma once

#include <map>

#include <glm/glm.hpp>

#include "block.h"
#include "drawing.h"

#define CHUNK_SIZE 8		// each chunk will be a column with this length and width
#define WORLD_HEIGHT 32		// height of the world 

class Chunk {
private:													// key is formatted as: (x << 16 + z), i.e. first 16 bits = x, second 16 bits = z
	Block* blocks[CHUNK_SIZE][WORLD_HEIGHT][CHUNK_SIZE];	// pointers to all blocks in this chunk at correct position
	Chunk* neighborChunks[4];		// pointers to surrounding chunks in order (front, right, back, left)
	glm::ivec3 pos;		// position of left, front corner (lowest x, z, y always 0) along integer grid (must be multiple of CHUNK_SIZE)
	std::vector<Vertex> verts;	// all vertices of all faces which should be drawn of blocks in this chunk
	bool updated;		// whether or not the block faces and verts of this chunk are up-to-date
	unsigned int vaoId, bufferId;		// id of the vao that holds this chunk
	glm::mat4 model;	// model matrix

	void addFace(const Vertex* face, int x, int y, int z);	// calculate and add the vertices for this face

	void updateBlockFaces();	// set which faces of each block are exposed
	void updateVerts();		// update the verts vector with the correct vertices
	void updateBuffer();		// update this chunk's buffer
public:
	static std::map<uint32_t, Chunk*> chunkList;		// a list of all the chunks mapped using a key based on chunk position
														// index is (x << 16 + z), i.e. first 16 bits are x, last 16 are z
	static void getChunkPosition(int global, int globalZ, int& chunk, int& chunkZ);	// gets the chunk position containing the global position (x, y, z), y = anything
	static void addBlock(int x, int y, int z, std::string textureName);	// add block to correct chunk at position (x, y, z) in global coords
	static void removeBlock(int x, int y, int z);	// remove and return the block at (x, y, z) in global coords
	static uint32_t getChunkIndex(int x, int z);	// returns the map key corresponding to this x and z
	
	Chunk(glm::ivec2 pos);	// create a chunk at the given (x, z)
	~Chunk();

	void addNeighbor(Chunk* chunk);		// add a neighboring chunk
	void update();		// update the block faces and vertices of this chunk
	bool isUpdated();	// whether or not the face/vertex data of this chunk is up to date

	glm::ivec3 getPosition();	// returns the position of this chunk
	unsigned int getVaoId();		// return the vertices array
	int getVertexCount();		// returns the total number of vertices of this chunk's vao
	glm::mat4 getModelMatrix();		// returns this chunk's model matrix
};