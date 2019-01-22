#ifndef CURSOR__H
#define CURSOR__H
#include <string>
#include "drawable.h"

class Cursor : public Drawable {
public:
  Cursor(const std::string&);
  Cursor(const std::string&, const Vector2f& pos, const Vector2f& vel, const Image*);
  Cursor(const Cursor&);
  virtual ~Cursor() { }
  Cursor& operator=(const Cursor&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const {
    return image->getSurface();
  }
  int getScaledWidth()  const { return getScale()*image->getWidth(); }
  int getScaledHeight() const { return getScale()*image->getHeight(); }

  void handleEvent(SDL_Event&);

  void right();
  void left();
  void up();
  void down();
  void stop();

private:
  const Image * image;

  //TODO: I need to change drawable to work with rects, since this is simpler to interface with the gameMap (will need overhaul of drawable.h)
  SDL_Rect tempRect;

protected:
  int worldWidth;
  int worldHeight;

  int getDistance(const Cursor*) const;
  Vector2f makeVelocity(int, int) const;
};
#endif
