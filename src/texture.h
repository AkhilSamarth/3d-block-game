#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>

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

// namespace for texture related stuff
namespace Texture {
	void loadTextures();	// all texture loading should be done here

	// things related to block textures
	std::map<std::string, BlockTexture>& getBlockTextureMap();	// returns the map which contains block names mapped to textures
	std::map<std::string, glm::ivec2>& getBlockOffsetMap();		// maps texture names to their offsets in the block spritesheet
	void addBlockTexture(std::string blockName, BlockTexture texture);		// add a block texture to the map
	void addBlockTextureOffset(std::string name, int uOffset, int vOffset);		// add a block texture name along with its offset in the spritesheet
	glm::ivec2 getBlockTextureOffset(std::string name);		// returns the right offset from the map

	// block spritesheet
	void loadBlockSpritesheet();	// load the spritesheet
	void bindBlockSpritesheet(unsigned int shaderId);		// binds the block spritesheet
	int getBlockSpritesheetWidth();	// returns the width of the spritesheet
	int getBlockSpritesheetHeight();	// returns the height of the spritesheet
	int getBlockSpritesheetUnit();		// returns the height/width of a single block in the spritesheet
}