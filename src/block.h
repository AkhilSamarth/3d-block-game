#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>

#define BIT_FACE_TOP 1
#define BIT_FACE_BOTTOM 2
#define BIT_FACE_FRONT 4
#define BIT_FACE_BACK 8
#define BIT_FACE_RIGHT 16
#define BIT_FACE_LEFT 32
#define BIT_FACE_ALL (64 - 1)

// forward declarations
struct Vertex;
struct BlockTexture;

class Block {
private:
	glm::ivec3 pos;		// position of left, bottom, front corner (lowest x, y, z) along integer grid
	unsigned char exposedFaces;		// 1 byte bitmask for which faces are exposed
	std::string name;	// name of this block
	
public:
	// vertex arrays which contain data for each face
	static const Vertex TOP_FACE[6];
	static const Vertex BOTTOM_FACE[6];
	static const Vertex FRONT_FACE[6];
	static const Vertex BACK_FACE[6];
	static const Vertex RIGHT_FACE[6];
	static const Vertex LEFT_FACE[6];

	static glm::ivec3 getBlockPositionFromGlobal(glm::vec3 pos);	// returns the block position that pos is in

	Block(std::string name, int x, int y, int z);		// position set to (x, y, z)

	std::string getName();		// returns the name of this block

	void setFace(unsigned char bits);		// sets which faces are exposed (e.g. setFaces(BIT_FACE_TOP | BIT_FACE_FRONT))
	void resetFace(unsigned char bits);		// sets which faces are not exposed (see example above)
	void boolFace(unsigned char bits, bool status);		// sets the given faces based on status
	bool getFace(unsigned char bits);		// gets which faces are exposed (if multiple bits supplied, returns true only if all given faces are exposed)
};