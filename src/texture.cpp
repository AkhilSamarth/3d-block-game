#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <GL/glew.h>

#include "texture.h"
#include "block.h"

#define BLOCK_SPRITE_UNIT 32	// height/width of one block in the spritesheet
#define BLOCK_SPRITE_NAME "block sprites"
#define BLOCK_SPRITE_PATH "assetts/textures/block_sprite.png"

namespace Texture {
	void loadTextures() {
		// block textures
		// sprite sheet offset (should have one entry for each block in the spritesheet)
		addBlockTextureOffset("dirt", 0, 0);
		addBlockTextureOffset("grass_side", 1, 0);
		addBlockTextureOffset("grass", 0, 1);
		addBlockTextureOffset("stone", 1, 1);

		// textures for each block
		addBlockTexture("dirt", BlockTexture("dirt"));
		addBlockTexture("stone", BlockTexture("stone"));
		addBlockTexture("grass", BlockTexture("grass", "dirt", "grass_side", "grass_side", "grass_side", "grass_side"));

		Texture::loadBlockSpritesheet();
	}

	std::map<std::string, BlockTexture>& getBlockTextureMap() {
		static std::map<std::string, BlockTexture> blockTextures;
		return blockTextures;
	}

	std::map<std::string, glm::ivec2>& getBlockOffsetMap() {
		static std::map<std::string, glm::ivec2> blockOffsets;
		return blockOffsets;
	}

	void addBlockTexture(std::string blockName, BlockTexture texture) {
		getBlockTextureMap().insert(std::pair<std::string, BlockTexture>(blockName, texture));
	}

	void addBlockTextureOffset(std::string name, int uOffset, int vOffset) {
		getBlockOffsetMap()[name] = glm::ivec2(uOffset, vOffset);
	}

	glm::ivec2 getBlockTextureOffset(std::string name) {
		// check if the name exists
		if (getBlockOffsetMap().find(name) == getBlockOffsetMap().end()) {
			std::cerr << "Texture name \"" << name << "\" not found." << std::endl;
			return glm::ivec2(0, 0);
		}

		return getBlockOffsetMap()[name];
	}

	static unsigned int blockSpriteId = 0;
	static int blockSpriteWidth = 0, blockSpriteHeight = 0;		// dimensions of the block spritesheet
	static bool blockSpriteLoaded = false;	// whether or not the block spritesheet has been loaded

	void loadBlockSpritesheet() {
		std::string path = BLOCK_SPRITE_PATH;	// do this so functions c_str can be used

		// load image
		int channels;
		stbi_set_flip_vertically_on_load(true);		// makes sure the image is the right way up
		unsigned char* data = stbi_load(path.c_str(), &blockSpriteWidth, &blockSpriteHeight, &channels, 0);

		// generate texture
		glGenTextures(1, &blockSpriteId);

		// bind texture and set parameters
		glBindTexture(GL_TEXTURE_2D, blockSpriteId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// fill with data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, blockSpriteWidth, blockSpriteHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		// delete data
		stbi_image_free(data);

		// update flag
		blockSpriteLoaded = true;
	}

	void bindBlockSpritesheet(unsigned int shaderId) {
		if (!blockSpriteLoaded) {
			std::cerr << "Attempted to bind block spritesheet without loading it in first!" << std::endl;
			return;
		}

		// activate texture unit and bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, blockSpriteId);

		// get uniform
		glUniform1i(glGetUniformLocation(shaderId, "texture"), 0);
	}

	int getBlockSpritesheetWidth() {
		return blockSpriteWidth;
	}

	int getBlockSpritesheetHeight() {
		return blockSpriteHeight;
	}

	int getBlockSpritesheetUnit() {
		return BLOCK_SPRITE_UNIT;
	}
}