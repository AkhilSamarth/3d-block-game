#include <iostream>

#include "chunk.h"

std::map<uint32_t, Chunk*> Chunk::chunkList = std::map<uint32_t, Chunk*>();

void Chunk::addBlock(int x, int y, int z, std::string textureName) {
	// calculate correct chunk position
	int chunkX = x - (x % CHUNK_SIZE);
	int chunkZ = z - (z % CHUNK_SIZE);

	// calculate chunk index for map
	uint32_t chunkIndex = getChunkIndex(chunkX, chunkZ);

	// check if a chunk exists at the given position, if not create it
	if (chunkList.find(chunkIndex) == chunkList.end()) {
		chunkList[chunkIndex] = new Chunk(glm::ivec2(chunkX, chunkZ));
	}

	// add block to the right chunk
	Chunk* chunk = chunkList[chunkIndex];
	chunk->blocks[x][y][z] = new Block(x - chunkX, y, z - chunkZ, textureName);

	// set update flag
	chunk->updated = false;
}

void Chunk::removeBlock(int x, int y, int z) {
	// calculate correct chunk position
	int chunkX = x - (x % CHUNK_SIZE);
	int chunkZ = z - (z % CHUNK_SIZE);

	// calculate chunk index for map
	uint32_t chunkIndex = getChunkIndex(chunkX, chunkZ);

	// check if a chunk exists at the given position
	if (chunkList.find(chunkIndex) == chunkList.end()) {
		std::cerr << "Chunk for block position (x: " << x << ", y: " << y << ", z: " << z << ") does not exist!" << std::endl;
		return;
	}

	// get block if it exists
	Chunk* chunk = chunkList[chunkIndex];
	Block* block = chunk->blocks[x][y][z];

	if (block == nullptr) {
		std::cerr << "No block found at position (x: " << x << ", y: " << y << ", z: " << z << ") does not exist!" << std::endl;
		return;
	}

	// remove block from array and free its memory
	chunk->blocks[x][y][z] = nullptr;
	delete block;

	// set update flag
	chunk->updated = false;
}

uint32_t Chunk::getChunkIndex(int x, int z) {
	return x << 16 + z;
}

Chunk::Chunk(glm::ivec2 pos) : blocks(), neighborChunks(), verts(std::vector<Vertex>()) {
	// check position
	if (pos.x % CHUNK_SIZE != 0 || pos.y % CHUNK_SIZE != 0) {
		std::cerr << "Invalid chunk position (x: " << pos.x << ", z: " << pos.y << ") given!" << std::endl;
		std::cerr << "This chunk will not work as expected!" << std::endl;

		return;
	}

	// set position
	this->pos = glm::ivec3(pos.x, 0, pos.y);

	// add to chunkList
	chunkList[getChunkIndex(pos.x, pos.y)] = this;

	// generate vao and set attributes
	glGenVertexArrays(1, &vaoId);
	glGenBuffers(1, &bufferId);

	// bind buffer to vao
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);

	// set vertex attribs
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 3);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

Chunk::~Chunk() {
	// remove this chunk from the list
	chunkList.erase(getChunkIndex(this->pos.x, this->pos.z));
}


void Chunk::updateBlockFaces() {
	// loop through all chunk blocks
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			for (int y = 0; y < WORLD_HEIGHT; y++) {
				// set face status of neighbor blocks depending on this one
				// if this block exists, neighbor blocks have hidden faces
				// if it doesn't exist, neighbor blocks have exposed faces
				bool exposed = (blocks[x][y][z] == nullptr);

				// set status of neighbor blocks, if they exist
				Block* neighbor;
				if (x + 1 < CHUNK_SIZE && (neighbor = blocks[x + 1][y][z]) != nullptr) {
					neighbor->boolFace(BIT_FACE_LEFT, exposed);
				}
				if (x - 1 >= 0 && (neighbor = blocks[x - 1][y][z]) != nullptr) {
					neighbor->boolFace(BIT_FACE_RIGHT, exposed);
				}
				if (y + 1 < WORLD_HEIGHT && (neighbor = blocks[x][y + 1][z]) != nullptr) {
					neighbor->boolFace(BIT_FACE_BOTTOM, exposed);
				}
				if (y - 1 >= 0 && (neighbor = blocks[x][y - 1][z]) != nullptr) {
					neighbor->boolFace(BIT_FACE_TOP, exposed);
				}
				if (z + 1 < CHUNK_SIZE && (neighbor = blocks[x][y][z + 1]) != nullptr) {
					neighbor->boolFace(BIT_FACE_FRONT, exposed);
				}
				if (z - 1 >= 0 && (neighbor = blocks[x][y][z - 1]) != nullptr) {
					neighbor->boolFace(BIT_FACE_BACK, exposed);
				}
			}
		}
	}
}

void Chunk::updateVerts() {
	verts.clear();

	// loop through all block positions
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			for (int y = 0; y < WORLD_HEIGHT; y++) {
				// if there's no block here, continue
				Block* block;
				if ((block = blocks[x][y][z]) == nullptr) {
					continue;
				}

				// add exposed faces
				if (block->getFace(BIT_FACE_TOP)) {
					verts.insert(verts.end(), Block::TOP_FACE, Block::TOP_FACE + 6);
				}
				if (block->getFace(BIT_FACE_BOTTOM)) {
					verts.insert(verts.end(), Block::BOTTOM_FACE, Block::BOTTOM_FACE + 6);
				}
				if (block->getFace(BIT_FACE_LEFT)) {
					verts.insert(verts.end(), Block::LEFT_FACE, Block::LEFT_FACE + 6);
				}
				if (block->getFace(BIT_FACE_RIGHT)) {
					verts.insert(verts.end(), Block::RIGHT_FACE, Block::RIGHT_FACE + 6);
				}
				if (block->getFace(BIT_FACE_FRONT)) {
					verts.insert(verts.end(), Block::FRONT_FACE, Block::FRONT_FACE + 6);
				}
				if (block->getFace(BIT_FACE_BACK)) {
					verts.insert(verts.end(), Block::BACK_FACE, Block::BACK_FACE + 6);
				}
			}
		}
	}
}

void Chunk::updateBuffer() {
	// update buffer with verts
	glNamedBufferData(bufferId, verts.size() * sizeof(Vertex), &verts[0], GL_DYNAMIC_DRAW);
}

void Chunk::update() {
	// don't do anything if update isn't needed
	if (updated) {
		return;
	}

	// call the update functions
	updateBlockFaces();
	updateVerts();
	updateBuffer();

	// set update flag
	updated = true;
}

bool Chunk::isUpdated() {
	return updated;
}

glm::ivec3 Chunk::getPosition() {
	return pos;
}

unsigned int Chunk::getVaoId() {
	// warn user if data is not up to date
	if (!updated) {
		std::cout << "Warning: vertex data of this chunk is not up to date" << std::endl;
	}

	return vaoId;
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