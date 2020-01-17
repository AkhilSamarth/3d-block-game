#pragma once

#include <string>
#include <map>

std::map<std::string, unsigned int>& getTextureMap();	// returns the map which contains all texture names mapped to their open gl ids
unsigned int getTextureId(std::string name);		// returns the id associated with this texture name
void bindTexture(std::string name, unsigned int shaderId);		// bind the given texture
unsigned int loadTexture(std::string path, std::string name);		// loads image at the given path as a texture and assigns it in the map to the given name, returns id

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
};