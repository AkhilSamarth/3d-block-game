#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <GL/glew.h>

#include "texture.h"

unsigned int getTextureId(std::string name) {
	if (textureMap.find(name) == textureMap.end()) {
		std::cerr << "Texture not found." << std::endl;
		return 0;
	}

	return textureMap[name];
}

void bindTexture(std::string name) {
	// get the texture id
	unsigned int textureId = getTextureId(name);
}

void loadTexture(std::string path, std::string name) {
	// load image
	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	// generate texture
	unsigned int textureId;
	glGenTextures(1, &textureId);

	// add id to map
	textureMap[name] = textureId;

	// bind texture and fill with data
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}