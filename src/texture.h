#pragma once

#include <string>
#include <map>

static std::map<std::string, int> textureMap;	// map which links texture names to their openGL id

unsigned int getTextureId(std::string name);		// returns the id associated with this texture name
void bindTexture(std::string name, unsigned int shaderId);		// bind the given texture
void loadTexture(std::string path, std::string name);		// loads image at the given path as a texture and assigns it in the map to the given name