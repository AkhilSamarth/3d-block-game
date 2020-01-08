#version 330 core

// color info coming in from vertex shader
in vec3 fragColor;

// final color to output to screen
out vec4 finalColor;

void main() {
	finalColor = vec4(fragColor, 1);
}