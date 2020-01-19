#include "terrain.h"

namespace Terrain {
	std::string flat10(int x, int y, int z) {
		// only y matters
		if (y < 6) {
			return "stone";
		}
		else if (y < 9) {
			return "dirt";
		}
		else  if (y < 10) {
			return "grass";
		}
		
		return "";
	}
}