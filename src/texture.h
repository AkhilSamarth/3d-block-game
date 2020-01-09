#pragma once

#include <string>
#include <map>

static std::map<std::string, int> textureMap;	// map which links texture names to their openGL id

unsigned int getTextureId(std::string name);		// returns the id associated with this texture name
unsigned char* loadTexture(std::string path);		// loads image at the given path as a texture