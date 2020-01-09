#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "block.h"
#include "drawing.h"

// initalize static members
unsigned int Block::vaoId = 0;
bool Block::vaoInit = false;

void Block::initVao() {
	// create and bind a VAO
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	// create and bind the VBO for this VAO
	unsigned int bufferId;
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);

	// data for a 1x1x1 cube with appropriate texture coordinates
	Vertex verts[36] = {
		// position				// texture coords
		// top
		-0.5, 0.5, -0.5,		0, 1,
		0.5, 0.5, -0.5,			1, 1,
		0.5, 0.5, 0.5,			1, 0,

		-0.5, 0.5, -0.5,		0, 1,
		0.5, 0.5, 0.5,			1, 0,
		-0.5, 0.5, 0.5,			0, 0,

		// bottom
		-0.5, -0.5, -0.5,		0, 1,
		0.5, -0.5, -0.5,		1, 1,
		0.5, -0.5, 0.5,			1, 0,

		-0.5, -0.5, -0.5, 		0, 1,
		0.5, -0.5, 0.5,			1, 0,
		-0.5, -0.5, 0.5,		0, 0,

		// front
		-0.5, 0.5, 0.5,			0, 1,
		0.5, 0.5, 0.5,			1, 1,
		0.5, -0.5, 0.5,			1, 0,

		-0.5, 0.5, 0.5,			0, 1,
		0.5, -0.5, 0.5,			1, 0,
		-0.5, -0.5, 0.5,		0,	0,

		// back
		-0.5, 0.5, -0.5,		0, 1,
		0.5, 0.5, -0.5,			1, 1,
		0.5, -0.5, -0.5, 		1, 0,

		-0.5, 0.5, -0.5,		0, 1,
		0.5, -0.5, -0.5,		1, 0,
		-0.5, -0.5, -0.5,		0,	0,


		// right
		0.5, 0.5, 0.5,			1, 1,
		0.5, 0.5, -0.5,			0, 1,
		0.5, -0.5, -0.5,		0, 0,

		0.5, 0.5, 0.5,			1, 1,
		0.5, -0.5, -0.5,		0, 0,
		0.5, -0.5, 0.5,			1, 0,

		// left
		-0.5, 0.5, 0.5,			1, 1,
		-0.5, 0.5, -0.5,		0, 1,
		-0.5, -0.5, -0.5,		0, 0,

		-0.5, 0.5, 0.5, 		1, 1,
		-0.5, -0.5, -0.5,		0, 0,
		-0.5, -0.5, 0.5,		1, 0
	};

	// fill buffer
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(Vertex), verts, GL_STATIC_DRAW);

	// vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (3 * sizeof(float)));	// texture coordinates
	glEnableVertexAttribArray(1);

	vaoInit = true;
}

Block::Block(int x, int y, int z, std::string textureName) : textureName(textureName) {
	pos = glm::vec3(x, y, z);
}

glm::mat4 Block::getModelMatrix() {
	glm::mat4 matrix = glm::mat4(1.0f);		// identity matrix
	matrix = glm::translate(matrix, glm::vec3(pos));		// translate to correct position

	return matrix;
}

void Block::bindTexture() {
	return;		// TODO: implement this
}

void Block::bindVao() {
	// if the vao doesn't exist yet, create it
	if (!vaoInit) {
		initVao();
	}

	glBindVertexArray(vaoId);
}