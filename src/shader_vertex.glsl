#version 330 core

// vertex attributes
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

// color to pass to fragment shader
out vec3 fragColor;	

void main() {
	gl_Position = vec4(pos, 1);
	fragColor = color;
}