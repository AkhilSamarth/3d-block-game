#version 330 core

// vertex attributes
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texturePos;

out vec2 texCoord;

// matrix transformations
uniform mat4 camera;	// includes view and projection
uniform mat4 model;

void main() {
	gl_Position = camera * model * vec4(pos, 1);
	texCoord = texturePos;
}