#pragma once

#include <string>
#include <map>

// simple struct to encapsulate all texture names for a block
struct BlockTexture {
	std::string top;
	std::string bottom;
	std::string left;
	std::string right;
	std::string front;
	std::string back;

	BlockTexture(std::string top, std::string bottom, std::string left, std::string right, std::string front, std::string back)
		: top(top), bottom(bottom), left(left), right(right), front(front), back(back) {}

	BlockTexture(std::string allSides)
		: top(allSides), bottom(allSides), left(allSides), right(allSides), front(allSides), back(allSides) {}
};

std::map<std::string, unsigned int>& getTextureMap();	// returns the map which contains all texture names mapped to their open gl ids

std::map<std::string, BlockTexture>& getBlockTextures();	// returns the map which contains block names mapped to textures
void addBlockTexture(std::string blockName, BlockTexture texture);		// add a block texture to the map

void loadTextures();	// all texture loading should be done here

unsigned int getTextureId(std::string name);		// returns the id associated with this texture name
void bindTexture(std::string name, unsigned int shaderId);		// bind the given texture
unsigned int loadTexture(std::string path, std::string name);		// loads image at the given path as a texture and assigns it in the map to the given name, returns id