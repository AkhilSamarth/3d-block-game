#include <iostream>

#include "textures.h"

void addTexture(std::string name, unsigned char* data) {
	textureMap[name] = data;
}

unsigned char* getTexture(std::string name) {
	if (textureMap.find(name) == textureMap.end()) {
		std::cerr << "Texture not found." << std::endl;
		return nullptr;
	}

	return textureMap[name];
}