#include "gameMap.h"
#include "imageFactory.h"

//Tile* tileset[192];
//SDL_Rect tileclips[TOTAL_TILE_SPRITES];

GameMap::GameMap():
//data driven
//needs total number of tiles
//height, width of the map
//x, y of the tiles
//file location to get the tile map from
//file location for the tileset
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

      std::cout << tileType << std::endl;

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
    /*
    //Clip the sprite sheet
    tileclips[TILE_RED]->x = 0;
    tileclips[TILE_RED]->y = 0;
    tileclips[TILE_RED]->w = TILE_WIDTH;
    tileclips[TILE_RED]->h = TILE_HEIGHT;

    tileclips[TILE_GREEN]->x = 0;
    tileclips[TILE_GREEN]->y = 80;
    tileclips[TILE_GREEN]->w = TILE_WIDTH;
    tileclips[TILE_GREEN]->h = TILE_HEIGHT;

    tileclips[TILE_BLUE]->x = 0;
    tileclips[TILE_BLUE]->y = 160;
    tileclips[TILE_BLUE]->w = TILE_WIDTH;
    tileclips[TILE_BLUE]->h = TILE_HEIGHT;

    tileclips[TILE_TOPLEFT]->x = 80;
    tileclips[TILE_TOPLEFT]->y = 0;
    tileclips[TILE_TOPLEFT]->w = TILE_WIDTH;
    tileclips[TILE_TOPLEFT]->h = TILE_HEIGHT;

    tileclips[TILE_LEFT]->x = 80;
    tileclips[TILE_LEFT]->y = 80;
    tileclips[TILE_LEFT]->w = TILE_WIDTH;
    tileclips[TILE_LEFT]->h = TILE_HEIGHT;

    tileclips[TILE_BOTTOMLEFT]->x = 80;
    tileclips[TILE_BOTTOMLEFT]->y = 160;
    tileclips[TILE_BOTTOMLEFT]->w = TILE_WIDTH;
    tileclips[TILE_BOTTOMLEFT]->h = TILE_HEIGHT;

    tileclips[TILE_TOP]->x = 160;
    tileclips[TILE_TOP]->y = 0;
    tileclips[TILE_TOP]->w = TILE_WIDTH;
    tileclips[TILE_TOP]->h = TILE_HEIGHT;

    tileclips[TILE_CENTER]->x = 160;
    tileclips[TILE_CENTER]->y = 80;
    tileclips[TILE_CENTER]->w = TILE_WIDTH;
    tileclips[TILE_CENTER]->h = TILE_HEIGHT;

    tileclips[TILE_BOTTOM]->x = 160;
    tileclips[TILE_BOTTOM]->y = 160;
    tileclips[TILE_BOTTOM]->w = TILE_WIDTH;
    tileclips[TILE_BOTTOM]->h = TILE_HEIGHT;

    tileclips[TILE_TOPRIGHT]->x = 240;
    tileclips[TILE_TOPRIGHT]->y = 0;
    tileclips[TILE_TOPRIGHT]->w = TILE_WIDTH;
    tileclips[TILE_TOPRIGHT]->h = TILE_HEIGHT;

    tileclips[TILE_RIGHT]->x = 240;
    tileclips[TILE_RIGHT]->y = 80;
    tileclips[TILE_RIGHT]->w = TILE_WIDTH;
    tileclips[TILE_RIGHT]->h = TILE_HEIGHT;

    tileclips[TILE_BOTTOMRIGHT]->x = 240;
    tileclips[TILE_BOTTOMRIGHT]->y = 160;
    tileclips[TILE_BOTTOMRIGHT]->w = TILE_WIDTH;
    tileclips[TILE_BOTTOMRIGHT]->h = TILE_HEIGHT;
    */
  }

  for (int i = 0; i < 12; i++) {
    std::cout << tileclips[i]->w << std::endl;
  }
  
  std::cout << "made it" << std::endl;
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
  //image->draw(100, 100, -view.getX(), -view.getY());
  //image->draw(0, 0, 0, 0);
  //render all the tiles on the map
  for (int i = 0; i < TOTAL_TILES; i++) {
    //check if the tile is within the viewport
    if (true) {
      //this tile needs to be rendered
      SDL_Point* center = NULL;
      image->draw((tileset[i]->getRect().x - view.getX()),(tileset[i]->getRect().y - view.getY()), tileclips[tileset[i]->getType()], 0.0, SDL_FLIP_NONE);
      //get its x, y
      //adjust it by the viewport location
      //render the id'ed tile from the clipped tiles at the converted location
    }
  }
}

void GameMap::update() {
  viewX = view.getX();
  viewY = view.getY();
}
