#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "block.h"
#include "camera.h"

// class for shader program
class Shader {
private:
	unsigned int progId;	// the opengl id of the shader program
	bool progInit;	// whether or not the program has been successfully linked yet
public:
	Shader();
	unsigned int getProgramId();		// returns the program id if it exists, otherwise prints error message
	void addShader(const std::string path, const GLenum type);	// adds the shader whose src code is at the given path and checks for erorrs
	void linkProgram();		// links the program after all shaders have been added and checks for errors
};

// standard vertex for use with VBOs
struct Vertex{
	float pos[3];		// position of vertex
	float texturePos[2];	// texture coordinates

	friend std::ostream& operator<<(std::ostream& out, Vertex& vert);
};

void drawChunks(unsigned int shaderId, glm::mat4& camMatrix);	// draw all the chunks in the chunk list