#include <iostream>
#include <queue>
#include <set>
#include <glm/gtc/matrix_transform.hpp>

#include "chunk.h"
#include "texture.h"

std::map<uint32_t, Chunk*> Chunk::chunkList = std::map<uint32_t, Chunk*>();

void Chunk::updateChunksByNeighbor(Chunk* start, bool& doneUpdating) {
	doneUpdating = false;

	// queue containing all chunks that need to be updated
	std::queue<Chunk*> chunksToGo = std::queue<Chunk*>();
	chunksToGo.push(start);

	// set which ensures that chunks aren't added twice to the queue
	std::set<Chunk*> chunkSet = std::set<Chunk*>();
	chunkSet.insert(start);

	while (!chunksToGo.empty()) {
		// process next chunk waiting in queue
		Chunk* current = chunksToGo.front();
		chunksToGo.pop();
		current->updateData();

		// add unupdated neighbors to queue
		for (int i = 0; i < 4; i++) {
			Chunk* neighbor = current->neighborChunks[i];
			if (neighbor != nullptr && !neighbor->dataUpdated && (chunkSet.find(neighbor) == chunkSet.end())) {
				chunksToGo.push(neighbor);
				chunkSet.insert(neighbor);
			}
		}
	}

	doneUpdating = true;
}

void Chunk::updateAllChunks(bool& doneUpdating) {
	doneUpdating = false;

	// loop through chunklist
	for (auto entry = chunkList.begin(); entry != chunkList.end(); entry++) {
		Chunk* chunk = entry->second;
		// check for null chunks
		if (chunk == nullptr) {
			std::cerr << "Warning: null chunk found in chunkList" << std::endl;
			continue;
		}

		chunk->updateData();
	}

	doneUpdating = true;
}

void Chunk::getChunkPosition(int globalX, int globalZ, int& chunkX, int& chunkZ) {
	// two separate cases for positive and negative
	if (globalX >= 0) {
		chunkX = globalX - (globalX % CHUNK_SIZE);
	}
	else {
		if (-globalX % CHUNK_SIZE == 0) {
			chunkX = globalX + (-globalX % CHUNK_SIZE);
		}
		else {
			chunkX = globalX + (-globalX % CHUNK_SIZE) - CHUNK_SIZE;
		}
	}

	if (globalZ >= 0) {
		chunkZ = globalZ - (globalZ % CHUNK_SIZE);
	}
	else {
		if (-globalZ % CHUNK_SIZE == 0) {
			chunkZ = globalZ + (-globalZ % CHUNK_SIZE);
		}
		else {
			chunkZ = globalZ + (-globalZ % CHUNK_SIZE) - CHUNK_SIZE;
		}
	}
}

uint32_t Chunk::getChunkIndex(int x, int z) {
	return (x << 16) + z;
}

void Chunk::markNeighborsForUpdate(int localX, int localZ) {
	// check x
	if (localX == 0) {
		// left edge
		if (neighborChunks[3] != nullptr) {
			neighborChunks[3]->dataUpdated = false;
			neighborChunks[3]->bufferUpdated = false;
		}
	}
	else if (localX == CHUNK_SIZE - 1) {
		// right edge
		if (neighborChunks[1] != nullptr) {
			neighborChunks[1]->dataUpdated = false;
			neighborChunks[1]->bufferUpdated = false;
		}
	}
	// check z
	if (localZ == 0) {
		// front edge
		if (neighborChunks[0] != nullptr) {
			neighborChunks[0]->dataUpdated = false;
			neighborChunks[0]->bufferUpdated = false;
		}
	}
	else if (localZ == CHUNK_SIZE - 1) {
		// back edge
		if (neighborChunks[2] != nullptr) {
			neighborChunks[2]->dataUpdated = false;
			neighborChunks[2]->bufferUpdated = false;
		}
	}
}

void Chunk::addBlock(std::string blockName, int x, int y, int z) {
	// make sure block is in bounds vertically
	if (y < 0 || y >= WORLD_HEIGHT) {
		std::cerr << "Attempted to add block out of bounds (y = " << y << ")." << std::endl;
		return;
	}

	// calculate correct chunk position
	int chunkX;
	int chunkZ;
	getChunkPosition(x, z, chunkX, chunkZ);

	// calculate chunk index for map
	uint32_t chunkIndex = getChunkIndex(chunkX, chunkZ);

	// check if a chunk exists at the given position, if not create it
	if (chunkList.find(chunkIndex) == chunkList.end()) {
		chunkList[chunkIndex] = new Chunk(glm::ivec2(chunkX, chunkZ));
	}

	// add block to the right chunk
	Chunk* chunk = chunkList[chunkIndex];
	chunk->blocks[x - chunkX][y][z - chunkZ] = new Block(blockName, x - chunkX, y, z - chunkZ);

	// set update flags
	chunk->dataUpdated = false;
	chunk->bufferUpdated = false;

	// update neighbors if needed
	chunk->markNeighborsForUpdate(x - chunkX, z - chunkZ);
}

void Chunk::removeBlock(int x, int y, int z) {
	// calculate correct chunk position
	int chunkX;
	int chunkZ;
	getChunkPosition(x, z, chunkX, chunkZ);

	// calculate chunk index for map
	uint32_t chunkIndex = getChunkIndex(chunkX, chunkZ);

	// check if a chunk exists at the given position
	if (chunkList.find(chunkIndex) == chunkList.end()) {
		std::cerr << "Chunk for block position (x: " << x << ", y: " << y << ", z: " << z << ") does not exist!" << std::endl;
		return;
	}

	// get block if it exists
	Chunk* chunk = chunkList[chunkIndex];
	Block* block = chunk->blocks[x - chunkX][y][z - chunkZ];

	if (block == nullptr) {
		std::cerr << "No block found at position (x: " << x << ", y: " << y << ", z: " << z << ")" << std::endl;
		return;
	}

	// remove block from array and free its memory
	chunk->blocks[x - chunkX][y][z - chunkZ] = nullptr;
	delete block;

	// set update flags
	chunk->dataUpdated = false;
	chunk->bufferUpdated = false;

	// update neighbors if needed
	chunk->markNeighborsForUpdate(x - chunkX, z - chunkZ);
}

bool Chunk::checkBlock(int x, int y, int z) {
	int chunkX;
	int chunkZ;
	getChunkPosition(x, z, chunkX, chunkZ);

	// calculate chunk index for map
	uint32_t chunkIndex = getChunkIndex(chunkX, chunkZ);

	// check if a chunk exists at the given position
	if (chunkList.find(chunkIndex) == chunkList.end()) {
		return false;
	}

	// return if a block exists
	return chunkList[chunkIndex]->blocks[x - chunkX][y][z - chunkZ] != nullptr;
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

	// set model matrix
	model = glm::translate(glm::mat4(1.0f), glm::vec3(this->pos));

	// add to chunkList
	chunkList[getChunkIndex(pos.x, pos.y)] = this;

	// check if neighbors exist, and if so, create a connection to them
	Chunk* neighbor;
	if (chunkList.find(getChunkIndex(pos.x, pos.y - CHUNK_SIZE)) != chunkList.end()) {
		// front
		neighbor = chunkList[getChunkIndex(pos.x, pos.y - CHUNK_SIZE)];
		neighborChunks[0] = neighbor;
		neighbor->addNeighbor(this);
	}
	if (chunkList.find(getChunkIndex(pos.x + CHUNK_SIZE, pos.y)) != chunkList.end()) {
		// right
		neighbor = chunkList[getChunkIndex(pos.x + CHUNK_SIZE, pos.y)];
		neighborChunks[1] = neighbor;
		neighbor->addNeighbor(this);
	}
	if (chunkList.find(getChunkIndex(pos.x, pos.y + CHUNK_SIZE)) != chunkList.end()) {
		// back
		neighbor = chunkList[getChunkIndex(pos.x, pos.y + CHUNK_SIZE)];
		neighborChunks[2] = neighbor;
		neighbor->addNeighbor(this);
	}
	if (chunkList.find(getChunkIndex(pos.x - CHUNK_SIZE, pos.y)) != chunkList.end()) {
		// left
		neighbor = chunkList[getChunkIndex(pos.x - CHUNK_SIZE, pos.y)];
		neighborChunks[3] = neighbor;
		neighbor->addNeighbor(this);
	}

	// generate vao and set attributes
	glGenVertexArrays(1, &vaoId);
	glGenBuffers(1, &bufferId);

	// bind buffer to vao
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);

	// set vertex attribs
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

Chunk::~Chunk() {
	// remove this chunk from the list
	chunkList.erase(getChunkIndex(pos.x, pos.z));
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

				// if this is the top or bottom, expose top/bottom face
				if (y == 0) {
					if (blocks[x][y][z] != nullptr) {
						blocks[x][y][z]->setFace(BIT_FACE_BOTTOM);
					}
				}
				else if (y == WORLD_HEIGHT - 1) {
					if (blocks[x][y][z] != nullptr) {
						blocks[x][y][z]->setFace(BIT_FACE_TOP);
					}
				}
			}
		}
	}

	// use neighbors to calculate exposed faces on chunk boundaries
	// front and back faces
	Chunk* front = neighborChunks[0];
	Chunk* back = neighborChunks[2];
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < WORLD_HEIGHT; y++) {
			// front
			if (blocks[x][y][0] != nullptr) {
				if (front != nullptr) {
					// check corresponding block on neighbor
					bool expose = (front->blocks[x][y][CHUNK_SIZE - 1] == nullptr);
					blocks[x][y][0]->boolFace(BIT_FACE_FRONT, expose);
				}
				else {
					// no neighbor, so expose face
					blocks[x][y][0]->setFace(BIT_FACE_FRONT);
				}
			}

			// back
			if (blocks[x][y][CHUNK_SIZE - 1] != nullptr) {
				if (back != nullptr) {
					// check corresponding block on neighbor
					bool expose = (back->blocks[x][y][0] == nullptr);
					blocks[x][y][CHUNK_SIZE - 1]->boolFace(BIT_FACE_BACK, expose);
				}
				else {
					// no neighbor, so expose face
					blocks[x][y][CHUNK_SIZE - 1]->setFace(BIT_FACE_BACK);
				}
			}
		}
	}

	// left and right faces
	Chunk* right = neighborChunks[1];
	Chunk* left = neighborChunks[3];
	for (int z = 0; z < CHUNK_SIZE; z++) {
		for (int y = 0; y < WORLD_HEIGHT; y++) {
			// left
			if (blocks[0][y][z] != nullptr) {
				if (left != nullptr) {
					// check corresponding block on neighbor
					bool expose = (left->blocks[CHUNK_SIZE - 1][y][z] == nullptr);
					blocks[0][y][z]->boolFace(BIT_FACE_LEFT, expose);
				}
				else {
					// no neighbor, so expose face
					blocks[0][y][z]->setFace(BIT_FACE_LEFT);
				}
			}

			// right
			if (blocks[CHUNK_SIZE - 1][y][z] != nullptr) {
				if (right != nullptr) {
					// check corresponding block on neighbor
					bool expose = (right->blocks[0][y][z] == nullptr);
					blocks[CHUNK_SIZE - 1][y][z]->boolFace(BIT_FACE_RIGHT, expose);
				}
				else {
					// no neighbor, so expose face
					blocks[CHUNK_SIZE - 1][y][z]->setFace(BIT_FACE_RIGHT);
				}
			}
		}
	}
}

void Chunk::addFace(const Vertex* face, int x, int y, int z, int uOffset, int vOffset) {
	// calculate the size of a single block in spritesheet coordinates
	static const float BLOCK_SIZE_X = 1.0f * Texture::getBlockSpritesheetUnit() / Texture::getBlockSpritesheetWidth();
	static const float BLOCK_SIZE_Y = 1.0f * Texture::getBlockSpritesheetUnit() / Texture::getBlockSpritesheetHeight();

	// loop through all 6 verts of this face
	for (const Vertex* vertPtr = face; vertPtr < face + 6; vertPtr++) {
		// new vertex which will be added to the verts list
		Vertex outVert = *vertPtr;

		// shift position to be at right spot
		outVert.pos[0] += 0.5 + x;
		outVert.pos[1] += 0.5 + y;
		outVert.pos[2] += 0.5 + z;

		// shift texture coords using offset
		outVert.texturePos[0] = BLOCK_SIZE_X * (outVert.texturePos[0] + uOffset);
		outVert.texturePos[1] = BLOCK_SIZE_Y * (outVert.texturePos[1] + vOffset);

		// add to verts
		verts.push_back(outVert);
	}
}

void Chunk::updateVerts() {
	verts.clear();

	// loop through all block positions
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			for (int y = 0; y < WORLD_HEIGHT; y++) {
				Block* block;
				if ((block = blocks[x][y][z]) == nullptr) {
					continue;
				}

				// if all faces are hidden, continue
				if (!block->getFace(BIT_FACE_ALL)) {
					continue;
				}

				// get texture
				if (Texture::getBlockTextureMap().find(block->getName()) == Texture::getBlockTextureMap().end()) {
					std::cerr << "Warning: block texture for block named \"" << block->getName() << "\" not found." << std::endl;
				}
				BlockTexture texture = Texture::getBlockTextureMap().at(block->getName());

				// this texture's position in the spritesheet
				glm::ivec2 textureOffset;

				// add exposed faces
				if (block->getFace(BIT_FACE_TOP)) {
					textureOffset = Texture::getBlockTextureOffset(texture.top);
					addFace(Block::TOP_FACE, x, y, z, textureOffset.x, textureOffset.y);
				}
				if (block->getFace(BIT_FACE_BOTTOM)) {
					textureOffset = Texture::getBlockTextureOffset(texture.bottom);
					addFace(Block::BOTTOM_FACE, x, y, z, textureOffset.x, textureOffset.y);
				}
				if (block->getFace(BIT_FACE_LEFT)) {
					textureOffset = Texture::getBlockTextureOffset(texture.left);
					addFace(Block::LEFT_FACE, x, y, z, textureOffset.x, textureOffset.y);
				}
				if (block->getFace(BIT_FACE_RIGHT)) {
					textureOffset = Texture::getBlockTextureOffset(texture.right);
					addFace(Block::RIGHT_FACE, x, y, z, textureOffset.x, textureOffset.y);
				}
				if (block->getFace(BIT_FACE_FRONT)) {
					textureOffset = Texture::getBlockTextureOffset(texture.front);
					addFace(Block::FRONT_FACE, x, y, z, textureOffset.x, textureOffset.y);
				}
				if (block->getFace(BIT_FACE_BACK)) {
					textureOffset = Texture::getBlockTextureOffset(texture.back);
					addFace(Block::BACK_FACE, x, y, z, textureOffset.x, textureOffset.y);
				}
			}
		}
	}
}

void Chunk::updateBuffer() {
	// if buffer is up to date, do nothing
	if (bufferUpdated) {
		return;
	}

	// if data hasn't been updated, warn user and stop
	if (!dataUpdated) {
		std::cerr << "Attempted to update buffer without updating data." << std::endl;
		return;
	}

	// if verts is empty, continue
	if (verts.empty()) {
		return;
	}

	// update buffer with verts
	glNamedBufferData(bufferId, verts.size() * sizeof(Vertex), &verts[0], GL_DYNAMIC_DRAW);

	// update flag
	bufferUpdated = true;
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

void Chunk::updateData() {
	// don't do anything if update isn't needed
	if (dataUpdated) {
		return;
	}

	// call the update functions
	updateBlockFaces();
	updateVerts();

	// if verts is empty, no need to do anything with this chunk
	if (verts.empty()) {
		return;
	}

	// set update flag
	dataUpdated = true;
}

bool Chunk::isDataUpdated() {
	return dataUpdated;
}

bool Chunk::isBufferUpdated() {
	return bufferUpdated;
}

glm::ivec3 Chunk::getPosition() {
	return pos;
}

unsigned int Chunk::getVaoId() {
	// warn user if data is not up to date
	if (!dataUpdated || !bufferUpdated) {
		std::cout << "Warning: this chunk is not up to date" << std::endl;
	}

	return vaoId;
}

int Chunk::getVertexCount() {
	return verts.size();
}

glm::mat4 Chunk::getModelMatrix() {
	return model;
}