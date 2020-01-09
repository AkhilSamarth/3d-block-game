#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>

class Block {
private:
	std::string textureName;	// name of texture
	glm::ivec3 pos;	// position of back, left, bottom corner (lowest x, y, z) along integer grid

	static unsigned int vaoId;	// VAO for a single block (cube)
	static bool vaoInit;	// whether or not the block VAO has been created

	static std::map<std::string, unsigned char*> textureMap;	// map which links texture names to pointers to their data

	static void initVao();	// initalize the VAO
public:
	Block(int x, int y, int z, std::string textureName);		// position set to (x, y, z), textureName must match key in textureMap
	glm::mat4 getModelMatrix();		// returns the model matrix with the correct block position
	void bindTexture();		// binds this block's texture

	static void bindVao();		// binds the block VAO
	static void addTexture(std::string name, unsigned char* data);	// adds pointer to texture data to map
	static unsigned char* getTexture(std::string name);		// returns the pointer associated with this name
};