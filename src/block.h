#pragma once

#include <string>
#include <glm/glm.hpp>

#define BIT_FACE_TOP 1
#define BIT_FACE_BOTTOM 2
#define BIT_FACE_FRONT 4
#define BIT_FACE_BACK 8
#define BIT_FACE_RIGHT 16
#define BIT_FACE_LEFT 32

class Block {
private:
	std::string textureName;	// name of texture
	glm::ivec3 pos;	// position of back, left, bottom corner (lowest x, y, z) along integer grid
	unsigned char exposedFaces;		// bitmask for which faces are exposed

	static unsigned int vaoId;	// VAO for a single block (cube)
	static bool vaoInit;	// whether or not the block VAO has been created

	static void initVao();	// initalize the VAO
public:
	Block(int x, int y, int z, std::string textureName);		// position set to (x, y, z), textureName must match key in textureMap
	std::string getTextureName();	// returns the name of the texture of this block

	static void bindVao();		// binds the block VAO
};