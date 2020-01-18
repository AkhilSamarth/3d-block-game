#pragma once

#include <map>

#include <glm/glm.hpp>

#include "block.h"
#include "drawing.h"

#define CHUNK_SIZE 8		// each chunk will be a column with this length and width
#define WORLD_HEIGHT 32		// height of the world 

// forward declaration
class Chunk;

// struct to hold pointers to neighbors
struct ChunkNeighbors {
	Chunk* front;
	Chunk* right;
	Chunk* back;
	Chunk* left;
};

class Chunk {
private:													// key is formatted as: (x << 16 + z), i.e. first 16 bits = x, second 16 bits = z
	Block* blocks[CHUNK_SIZE][WORLD_HEIGHT][CHUNK_SIZE];	// pointers to all blocks in this chunk at correct position
	ChunkNeighbors neighborChunks;		// pointers to surrounding chunks in order (front, right, back, left)
	glm::ivec3 pos;		// position of left, front corner (lowest x, z, y always 0) along integer grid (must be multiple of CHUNK_SIZE)
	std::vector<Vertex> verts;	// all vertices of all faces which should be drawn of blocks in this chunk
	bool dataUpdated;		// whether or not the block faces and verts of this chunk are up-to-date
	bool bufferUpdated;		// whether or not the buffer is up to date
	unsigned int vaoId, bufferId;		// id of the vao that holds this chunk
	glm::mat4 model;	// model matrix

	void addNeighbor(Chunk* chunk);		// add a neighboring chunk
	void addFace(const Vertex* face, int x, int y, int z, int uOffset, int vOffset);	// calculate and add the vertices for this face, (x, y, z) = local position, x/y Offset = position in block spritesheet 

	void updateBlockFaces();	// set which faces of each block are exposed
	void updateVerts();		// update the verts vector with the correct vertices

	void markNeighborsForUpdate(int localX, int localZ);	// marks the neighbor chunks of the block at (localX, localZ) in local coords (0 to CHUNK_SIZE) for update if needed

public:
	static std::map<uint32_t, Chunk*> chunkList;		// a list of all the chunks mapped using a key based on chunk position
														// index is (x << 16 + z), i.e. first 16 bits are x, last 16 are z
	static void updateChunksByNeighbor(Chunk* start, bool& doneUpdating);	// updates chunks in a breadth-first-search style, starting with the given node. doneUpdating changes when updating is done
	static void updateAllChunks(bool& doneUpdating);		// updates all the chunks in the chunk list. doneUpdating changes when updating is done

	static void getChunkPosition(int global, int globalZ, int& chunk, int& chunkZ);	// gets the chunk position containing the global position (x, y, z), y = anything
	static uint32_t getChunkIndex(int x, int z);	// returns the map key corresponding to this x and z

	static void addBlock(std::string blockName, int x, int y, int z);	// add the given block to correct chunk at position (x, y, z) in global coords
	static void removeBlock(int x, int y, int z);	// remove the block at (x, y, z) in global coords
	static bool checkBlock(int x, int y, int z);	// returns whether or not a block exists at the given global coords

	Chunk(glm::ivec2 pos);	// create a chunk at the given (x, z)
	~Chunk();

	void updateData();		// update the block faces and vertices of this chunk
	void updateBuffer();		// update this chunk's buffer
	bool isDataUpdated();	// whether or not the face/vertex data of this chunk is up to date
	bool isBufferUpdated();	// whether or not the buffer is up to date

	glm::ivec3 getPosition();	// returns the position of this chunk
	unsigned int getVaoId();		// return the vertices array
	int getVertexCount();		// returns the total number of vertices of this chunk's vao
	glm::mat4 getModelMatrix();		// returns this chunk's model matrix
};