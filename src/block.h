#pragma once

#include <glm/glm.hpp>

class Block {
private:
	unsigned char* texture;	// pointer to texture data
	glm::ivec3 pos;	// position of back, left, bottom corner (lowest x, y, z) along integer grid

	static unsigned int vaoId;	// VAO for a single block (cube)
	static bool vaoInit;	// whether or not the block VAO has been created

	static void initVao();	// initalize the VAO
public:
	Block(int x, int y, int z, unsigned char* texture);		// position set to (x, y, z)
	glm::mat4 getModelMatrix();		// returns the model matrix with the correct block position
	void bindTexture();		// binds this block's texture

	static void bindVao();		// binds the block VAO
};