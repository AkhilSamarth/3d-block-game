#version 330 core

// texture coordinate
in vec2 texCoord;

// final color to output to screen
out vec4 finalColor;

uniform sampler2D texture;

void main() {
	finalColor = texture(texture, texCoord);
}