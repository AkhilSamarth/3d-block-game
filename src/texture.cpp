#include <iostream>
#include <stb_image.h>

#include "texture.h"

unsigned int getTextureId(std::string name) {
	if (textureMap.find(name) == textureMap.end()) {
		std::cerr << "Texture not found." << std::endl;
		return 0;
	}

	return textureMap[name];
}

unsigned char* loadTexture(std::string path) {
	
}