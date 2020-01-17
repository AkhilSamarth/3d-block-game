#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>

#define BIT_FACE_TOP 1
#define BIT_FACE_BOTTOM 2
#define BIT_FACE_FRONT 4
#define BIT_FACE_BACK 8
#define BIT_FACE_RIGHT 16
#define BIT_FACE_LEFT 32
#define BIT_FACE_ALL (64 - 1)

#define BLOCK_SPRITE_UNIT 64	// height/width of one block in the spritesheet

#define BLOCK_SPRITE_NAME "block sprites"
#define BLOCK_SPRITE_PATH "assetts/textures/block_sprite.png"

// forward declarations
struct Vertex;

// simple struct to encapsulate all texture names for a block
struct BlockTexture {
	std::string top;
	std::string bottom;
	std::string left;
	std::string right;
	std::string front;
	std::string back;

	BlockTexture(std::string top, std::string bottom, std::string left, std::string right, std::string front, std::string back)
		: top(top), bottom(bottom), left(left), right(right), front(front), back(back) {}

	BlockTexture(std::string allSides)
		: top(allSides), bottom(allSides), left(allSides), right(allSides), front(allSides), back(allSides) {}
};

class Block {
private:
	glm::ivec3 pos;		// position of left, bottom, front corner (lowest x, y, z) along integer grid
	unsigned char exposedFaces;		// 1 byte bitmask for which faces are exposed
	BlockTexture texture;	// texture for block

	static bool spriteLoaded;	// whether or not the spritesheet has been loaded
	static int spriteWidth, spriteHeight;	// dimensions of sprite sheet
	static std::map<std::string, glm::ivec2> blockOffsets;		// maps texture names to their offsets in the block spritesheet
public:
	// vertex arrays which contain data for each face
	static const Vertex TOP_FACE[6];
	static const Vertex BOTTOM_FACE[6];
	static const Vertex FRONT_FACE[6];
	static const Vertex BACK_FACE[6];
	static const Vertex RIGHT_FACE[6];
	static const Vertex LEFT_FACE[6];

	static void addBlockTextureOffset(std::string name, int uOffset, int vOffset);	// add a block texture name along with its offset in the spritesheet
	static glm::ivec2 getBlockTextureOffset(std::string name);	// returns the right offset from the map
	static void loadSpritesheet();	// load the spritesheet
	static void bindSpritesheet(unsigned int shaderId);		// binds the block spritesheet
	static int getSpriteWidth();	// returns the width of the spritesheet
	static int getSpriteHeight();	// returns the height of the spritesheet

	Block(int x, int y, int z, BlockTexture texture);		// position set to (x, y, z)

	BlockTexture getTexture();		// returns the texture

	void setFace(unsigned char bits);		// sets which faces are exposed (e.g. setFaces(BIT_FACE_TOP | BIT_FACE_FRONT))
	void resetFace(unsigned char bits);		// sets which faces are not exposed (see example above)
	void boolFace(unsigned char bits, bool status);		// sets the given faces based on status
	bool getFace(unsigned char bits);		// gets which faces are exposed (if multiple bits supplied, returns true only if all given faces are exposed)
};