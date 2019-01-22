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

    //TODO: probably better to use the viewport for this - so this is to test
    //sets the rect for the map
    tempRect.x = tempRect.y = 0;
    tempRect.h = MAP_HEIGHT;
    tempRect.w = MAP_WIDTH;

    //verify all the constants make sense
    //TODO

    tileset.reserve(TOTAL_TILES);
    TOTAL_SPRITES = (SPRITES_WIDTH/TILE_WIDTH) * (SPRITES_HEIGHT/TILE_HEIGHT);
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

    for (int i = 0; i < (SPRITES_HEIGHT/TILE_HEIGHT); i++) {
      for (int j = 0; j < (SPRITES_WIDTH/TILE_WIDTH); j++) {
        tileclips.push_back(new SDL_Rect);
        int tileNumber = (SPRITES_WIDTH/TILE_WIDTH) * i + j;
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

Tile* GameMap::getTileByIndex(int x, int y) {
  return tileset[(y*(MAP_WIDTH/TILE_WIDTH)+x)];
}

Tile* GameMap::getTileByCoord(int x, int y) {
  return getTileByIndex(x / TILE_WIDTH, y / TILE_HEIGHT);
}


//TODO: need a rewrite on this - just for testing right now
//check if the two rects are intersecting
bool GameMap::checkCollision(SDL_Rect a, SDL_Rect b) {
  //The sides of the rectangles
  int leftA, leftB;
  int rightA, rightB;
  int topA, topB;
  int bottomA, bottomB;

  //Calculate the sides of rect A
  leftA = a.x;
  rightA = a.x + a.w;
  topA = a.y;
  bottomA = a.y + a.h;

  //Calculate the sides of rect B
  leftB = b.x;
  rightB = b.x + b.w;
  topB = b.y;
  bottomB = b.y + b.h;

  //If any of the sides from A are outside of B
  if (bottomA <= topB)
  {
    return false;
  }

  if (topA >= bottomB)
  {
    return false;
  }

  if (rightA <= leftB)
  {
    return false;
  }

  if (leftA >= rightB)
  {
    return false;
  }

  //If none of the sides from A are outside B
  return true;
}

//if the rect is sticking out of bounds or into a tile it isn't allowed to enter, return false
bool GameMap::canEnter(SDL_Rect r) {
  return checkCollision(r, tempRect);
}
