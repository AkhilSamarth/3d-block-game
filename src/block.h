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
	glm::ivec3 pos;		// position of back, left, bottom corner (lowest x, y, z) along integer grid
	unsigned char exposedFaces;		// bitmask for which faces are exposed
public:
	// vertex arrays which contain data for each face
	static const Vertex TOP_FACE[6];
	static const Vertex BOTTOM_FACE[6];
	static const Vertex FRONT_FACE[6];
	static const Vertex BACK_FACE[6];
	static const Vertex RIGHT_FACE[6];
	static const Vertex LEFT_FACE[6];

	Block(int x, int y, int z, std::string textureName);		// position set to (x, y, z), textureName must match key in textureMap
	std::string getTextureName();	// returns the name of the texture of this block
};