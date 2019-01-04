#include "tile.h"

Tile::Tile(int x, int y, int w, int h, int t) {
	type = t;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}