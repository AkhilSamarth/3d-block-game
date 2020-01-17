#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "block.h"
#include "drawing.h"
#include "texture.h"

std::map<std::string, glm::ivec2> Block::blockOffsets = std::map<std::string, glm::ivec2>();

const unsigned int Block::SPRITE_ID = loadTexture("assetts/textures/block_sprite.png", "block sprites");

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

void Block::addBlockTexture(std::string name, int xOffset, int yOffset) {
	blockOffsets[name] = glm::ivec2(xOffset, yOffset);
}

Block::Block(int x, int y, int z, std::string textureName) : textureName(textureName), pos(glm::vec3(x, y, z)), exposedFaces(0) {}

std::string Block::getTextureName() {
	return textureName;
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