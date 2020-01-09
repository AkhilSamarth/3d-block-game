#pragma once

#include <string>
#include <map>

static std::map<std::string, unsigned char*> textureMap;	// map which links texture names to pointers to their data

static void addTexture(std::string name, unsigned char* data);	// adds pointer to texture data to map
static unsigned char* getTexture(std::string name);		// returns the pointer associated with this name
