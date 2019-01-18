#ifndef GAMEMAP__H
#define GAMEMAP__H

#include <string>
#include <vector>
#include "tile.h"
#include "viewport.h"

//The different tile sprites
const int TILE_RED = 0; //0 -
const int TILE_GREEN = 1; //4 -
const int TILE_BLUE = 2; //8 -
const int TILE_CENTER = 3; //6 -
const int TILE_TOP = 4; //2 -
const int TILE_TOPRIGHT = 5; //3 -
const int TILE_RIGHT = 6; //7 -
const int TILE_BOTTOMRIGHT = 7; //11 -
const int TILE_BOTTOM = 8; //10 -
const int TILE_BOTTOMLEFT = 9; //9 -
const int TILE_LEFT = 10; //5 -
const int TILE_TOPLEFT = 11; //1 -

class GameMap
{
public:
  static GameMap& getInstance();
  ~GameMap();

  //draw all of the tiles
  void draw() const;
  //update what gets shown by the moving viewport
  void update();
  bool canEnter(int, int);

private:
  GameMap();

  std::vector<Tile*> tileset;
  std::vector<SDL_Rect*> tileclips;
  Image* const image;
  const Viewport & view;

  float viewX;
  float viewY;

  int TOTAL_TILES;
  int MAP_WIDTH;
  int MAP_HEIGHT;
  int SPRITES_WIDTH;
  int SPRITES_HEIGHT;
  int TILE_WIDTH;
  int TILE_HEIGHT;
};

#endif

