#ifndef TILE__H
#define TILE__H

#include <SDL.h>

//stores a rectangle and type identifier for a tile
class Tile {
public:
	Tile(int x, int y, int w, int h, int type);
	int getType() const { return type;  }
	SDL_Rect getRect() const { return rect;  }
private:
	unsigned int type;
	SDL_Rect rect;
};

#endif