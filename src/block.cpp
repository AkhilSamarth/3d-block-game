#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include "block.h"
#include "drawing.h"
#include "texture.h"

std::map<std::string, glm::ivec2> Block::blockOffsets = std::map<std::string, glm::ivec2>();

bool Block::spriteLoaded = false;
int Block::spriteWidth = 0;
int Block::spriteHeight = 0;

// fill data arrays for a block that's centered at (0, 0, 0)
// arranged as:
//			position				texture coords
const Vertex Block::TOP_FACE[6] = { 
			0.5, 0.5, 0.5,			1, 0,
			0.5, 0.5, -0.5,			1, 1,
			-0.5, 0.5, -0.5,		0, 1,

			-0.5, 0.5, 0.5,			0, 0,
			0.5, 0.5, 0.5,			1, 0,
			-0.5, 0.5, -0.5,		0, 1};

const Vertex Block::BOTTOM_FACE[6] = {
			-0.5, -0.5, -0.5,		0, 1,
			0.5, -0.5, -0.5,		1, 1,
			0.5, -0.5, 0.5,			1, 0,

			-0.5, -0.5, -0.5,		0, 1,
			0.5, -0.5, 0.5,			1, 0,
			-0.5, -0.5, 0.5,		0, 0};

const Vertex Block::BACK_FACE[6] = { 
			0.5, -0.5, 0.5,			1, 0,
			0.5, 0.5, 0.5,			1, 1,
			-0.5, 0.5, 0.5,			0, 1,

			-0.5, -0.5, 0.5,		0, 0,
			0.5, -0.5, 0.5,			1, 0,
			-0.5, 0.5, 0.5,			0, 1};

const Vertex Block::FRONT_FACE[6] = {
			-0.5, 0.5, -0.5,		0, 1,
			0.5, 0.5, -0.5,			1, 1,
			0.5, -0.5, -0.5,		1, 0,

			-0.5, 0.5, -0.5,		0, 1,
			0.5, -0.5, -0.5,		1, 0,
			-0.5, -0.5, -0.5,		0, 0};

const Vertex Block::RIGHT_FACE[6] = {
			0.5, -0.5, -0.5,		0, 0,
			0.5, 0.5, -0.5,			0, 1,
			0.5, 0.5, 0.5,			1, 1,

			0.5, -0.5, 0.5,			1, 0,
			0.5, -0.5, -0.5,		0, 0,
			0.5, 0.5, 0.5,			1, 1};

const Vertex Block::LEFT_FACE[6] = {
			-0.5, 0.5, 0.5,			1, 1,
			-0.5, 0.5, -0.5,		0, 1,
			-0.5, -0.5, -0.5,		0, 0,

			-0.5, 0.5, 0.5,			1, 1,
			-0.5, -0.5, -0.5,		0, 0,
			-0.5, -0.5, 0.5,		1, 0};

void Block::loadSpritesheet() {
	std::string path = BLOCK_SPRITE_PATH;	// do this so functions c_str can be used

	// load image
	int channels;
	stbi_set_flip_vertically_on_load(true);		// makes sure the image is the right way up
	unsigned char* data = stbi_load(path.c_str(), &spriteWidth, &spriteHeight, &channels, 0);

	// generate texture
	unsigned int textureId;
	glGenTextures(1, &textureId);

	// add id to map
	getTextureMap()[BLOCK_SPRITE_NAME] = textureId;

	// bind texture and set parameters
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// fill with data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, spriteWidth, spriteHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	// delete data
	stbi_image_free(data);

	// update flag
	spriteLoaded = true;
}

void Block::addBlockTextureOffset(std::string name, int uOffset, int vOffset) {
	blockOffsets[name] = glm::ivec2(uOffset, vOffset);
}

glm::ivec2 Block::getBlockTextureOffset(std::string name) {
	// check if the name exists
	if (blockOffsets.find(name) == blockOffsets.end()) {
		std::cerr << "Texture name \"" << name << "\" not found." << std::endl;
		return glm::ivec2(0, 0);
	}

	return blockOffsets[name];
}

void Block::bindSpritesheet(unsigned int shaderId) {
	if (!spriteLoaded) {
		std::cerr << "Attempted to bind block spritesheet without loading it in first!" << std::endl;
		return;
	}

	bindTexture(BLOCK_SPRITE_NAME, shaderId);
}

int Block::getSpriteWidth() {
	return spriteWidth;
}

int Block::getSpriteHeight() {
	return spriteHeight;
}

Block::Block(int x, int y, int z, std::string texture) : pos(glm::vec3(x, y, z)), exposedFaces(0) {
	// set textures of all sides
	for (int i = 0; i < 6; i++) {
		textures[i] = texture;
	}
}

void Block::setFace(unsigned char bits) {
	exposedFaces |= bits;
}

void Block::resetFace(unsigned char bits) {
	exposedFaces &= ~bits;
}

void Block::boolFace(unsigned char bits, bool status) {
	if (status) {
		setFace(bits);
	}
	else {
		resetFace(bits);
	}
}

bool Block::getFace(unsigned char bits) {
	return exposedFaces & bits;
}