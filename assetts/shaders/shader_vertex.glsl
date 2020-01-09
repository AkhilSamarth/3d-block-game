#version 330 core

// vertex attributes
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texturePos;

out vec2 texCoord;

// matrix transformation
uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(pos, 1);
	texCoord = texturePos;
}