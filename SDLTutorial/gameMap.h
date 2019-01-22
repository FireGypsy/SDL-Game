#ifndef GAMEMAP__H
#define GAMEMAP__H

#include <string>
#include <vector>
#include "tile.h"
#include "viewport.h"

class GameMap
{
public:
  static GameMap& getInstance();
  ~GameMap();

  //draw all of the tiles
  void draw() const;
  //update what gets shown by the moving viewport
  void update();
  //returns the tile at int x, int y
  Tile* getTileByIndex(int x, int y);
  Tile* getTileByCoord(int x, int y);

  //TODO: need to make this contract clearer - checking if the sprite can validly enter a tile
  bool canEnter(SDL_Rect);
  bool checkCollision(SDL_Rect, SDL_Rect);

private:
  GameMap();

  //tiles go in left to right order, then row by row
  std::vector<Tile*> tileset;
  std::vector<SDL_Rect*> tileclips;
  Image* const image;
  const Viewport & view;

  float viewX;
  float viewY;

  SDL_Rect tempRect;

  int TOTAL_TILES;    //total number of tiles in the map
  int MAP_WIDTH;      //width of the map in pixels
  int MAP_HEIGHT;     //height of the map in pixels
  int SPRITES_WIDTH;  //width of the sprite sheet in pixels
  int SPRITES_HEIGHT; //height of the sprite sheet in pixels
  int TILE_WIDTH;     //width of the sprites in pixels
  int TILE_HEIGHT;    //height of the sprites in pixels

  //TODO: add values for width/height of the map and spritesheet, so I don't have to calculate that everytime
};

#endif

