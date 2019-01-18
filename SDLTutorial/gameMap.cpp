#include "gameMap.h"
#include "imageFactory.h"

GameMap& GameMap::getInstance() {
  static GameMap instance;
  return instance;
}

GameMap::GameMap():
image(ImageFactory::getInstance()->getImage("map")),
view(Viewport::getInstance()),
viewX(0.0), viewY(0.0),
TOTAL_TILES(0),
MAP_WIDTH(0),
MAP_HEIGHT(0),
SPRITES_WIDTH(0),
SPRITES_HEIGHT(0),
TILE_WIDTH(0),
TILE_HEIGHT(0)
{
  //The tile offsets
  int x = 0, y = 0;
  int TOTAL_SPRITES = 0;

  //Open the map
  std::ifstream map("test2.map");
  
  //If the map couldn't be loaded
  if (!map.is_open())
  {
    printf("Unable to load map file!\n");
  }
  //load the tiles from the file and the map from a diff file
  else
  {
    //Initialize all constants from the map file+
    map >> TOTAL_TILES;
    map >> MAP_WIDTH >> MAP_HEIGHT;

    map >> SPRITES_WIDTH >> SPRITES_HEIGHT;
map >> TILE_WIDTH >> TILE_HEIGHT;

//verify all the constants make sense
//TODO

tileset.reserve(TOTAL_TILES);
TOTAL_SPRITES = SPRITES_WIDTH * SPRITES_HEIGHT;
tileclips.reserve(TOTAL_SPRITES);

//Initialize the tiles
for (int i = 0; i < TOTAL_TILES; ++i)
{
  //Determines what kind of tile will be made
  int tileType = -1;

  //Read tile from map file
  map >> tileType;

  //If the was a problem in reading the map
  if (map.fail())
  {
    //Stop loading map
    printf("Error loading map: Unexpected end of file!\n");
    break;
  }

  //If the number is a valid tile number
  if ((tileType >= 0) && (tileType < TOTAL_SPRITES))
  {
    tileset.push_back(new Tile(x, y, TILE_WIDTH, TILE_HEIGHT, tileType));
  }
  //If we don't recognize the tile type
  else
  {
    //Stop loading map
    printf("Error loading map: Invalid tile type at %d!\n", i);
    break;
  }

  //Move to next tile spot
  x += TILE_WIDTH;

  //If we've gone too far
  if (x >= MAP_WIDTH)
  {
    //Move back
    x = 0;

    //Move to the next row
    y += TILE_HEIGHT;
  }

}

for (int i = 0; i < SPRITES_HEIGHT; i++) {
  for (int j = 0; j < SPRITES_WIDTH; j++) {
    tileclips.push_back(new SDL_Rect);
    int tileNumber = SPRITES_WIDTH * i + j;
    tileclips[tileNumber]->x = TILE_WIDTH * j;
    tileclips[tileNumber]->y = TILE_HEIGHT * i;
    tileclips[tileNumber]->w = TILE_WIDTH;
    tileclips[tileNumber]->h = TILE_HEIGHT;
  }
}
  }

  //Close the file
  map.close();
}

GameMap::~GameMap()
{
  //delete the map and the clipped tileset
  for (int i = 0; i < TOTAL_TILES; ++i) {
    delete tileset[i];
  }
}

void GameMap::draw() const {
  //render all the tiles on the map
  for (int i = 0; i < TOTAL_TILES; i++) {
    //check if the tile is within the viewport
    if (true) {
      //this tile needs to be rendered
      SDL_Point* center = NULL;
      image->draw((tileset[i]->getRect().x - view.getX()), (tileset[i]->getRect().y - view.getY()), tileclips[tileset[i]->getType()], 0.0, SDL_FLIP_NONE);
    }
  }
}

void GameMap::update() {
  viewX = view.getX();
  viewY = view.getY();
}

bool GameMap::canEnter(int x, int y) {
  //normalize it to the grid
  x = x / TILE_WIDTH;
  y = y / TILE_HEIGHT;

  if (tileset[y*(MAP_WIDTH / TILE_WIDTH) + x]->getType() == 0){
    return true;
  }
  return false;
}
