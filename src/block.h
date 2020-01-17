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

// forward declare Vertex
struct Vertex;

class Block {
private:
	std::string textureName;	// name of texture
	glm::ivec3 pos;		// position of left, bottom, front corner (lowest x, y, z) along integer grid
	unsigned char exposedFaces;		// 1 byte bitmask for which faces are exposed

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

	Block(int x, int y, int z, std::string textureName);		// position set to (x, y, z), textureName must match key in textureMap
	std::string getTextureName();	// returns the name of the texture of this block

	void setFace(unsigned char bits);		// sets which faces are exposed (e.g. setFaces(BIT_FACE_TOP | BIT_FACE_FRONT))
	void resetFace(unsigned char bits);		// sets which faces are not exposed (see example above)
	void boolFace(unsigned char bits, bool status);		// sets the given faces based on status
	bool getFace(unsigned char bits);		// gets which faces are exposed (if multiple bits supplied, returns true only if all given faces are exposed)
};