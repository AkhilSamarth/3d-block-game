#pragma once

#include <string>
#include <glm/glm.hpp>

class Block {
private:
	std::string textureName;	// name of texture
	glm::ivec3 pos;	// position of back, left, bottom corner (lowest x, y, z) along integer grid

	static unsigned int vaoId;	// VAO for a single block (cube)
	static bool vaoInit;	// whether or not the block VAO has been created


	static void initVao();	// initalize the VAO
public:
	Block(int x, int y, int z, std::string textureName);		// position set to (x, y, z), textureName must match key in textureMap
	glm::mat4 getModelMatrix();		// returns the model matrix with the correct block position
	void bindTexture();		// binds this block's texture

	static void bindVao();		// binds the block VAO
};