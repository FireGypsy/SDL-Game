#include <cmath>
#include <random>
#include <functional>
#include "cursor.h"
#include "gameData.h"
#include "imageFactory.h"

Vector2f Cursor::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx - 50, vx + 50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy - 50, vy + 50);;
  newvx *= []() { if (rand() % 2) return -1; else return 1; }();
  newvy *= []() { if (rand() % 2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

Cursor::Cursor(const string& n, const Vector2f& pos, const Vector2f& vel,
  const Image* img) :
  Drawable(n, pos, vel),
  image(img),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Cursor::Cursor(const std::string& name) :
  Drawable(name,
    Vector2f(Gamedata::getInstance().getXmlInt(name + "/startLoc/x"),
      Gamedata::getInstance().getXmlInt(name + "/startLoc/y")),
    Vector2f(
      Gamedata::getInstance().getXmlInt(name + "/speedX"),
      Gamedata::getInstance().getXmlInt(name + "/speedY"))
  ),
  image(ImageFactory::getInstance()->getImage(name)),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Cursor::Cursor(const Cursor& s) :
  Drawable(s),
  image(s.image),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Cursor& Cursor::operator=(const Cursor& rhs) {
  Drawable::operator=(rhs);
  image = rhs.image;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

inline namespace {
  constexpr float SCALE_EPSILON = 2e-7;
}

void Cursor::draw() const {
  if (getScale() < SCALE_EPSILON) return;
  image->draw(getX(), getY(), getScale());
}

void Cursor::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if (getY() < 0) {
    setVelocityY(std::abs(getVelocityY()));
  }
  if (getY() > worldHeight - getScaledHeight()) {
    setVelocityY(-std::abs(getVelocityY()));
  }

  if (getX() < 0) {
    setVelocityX(std::abs(getVelocityX()));
  }
  if (getX() > worldWidth - getScaledWidth()) {
    setVelocityX(-std::abs(getVelocityX()));
  }
}

void Cursor::handleEvent(SDL_Event& event) {
  if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
    if (event.key.keysym.sym == SDLK_w) {
      up();
    }
    if (event.key.keysym.sym == SDLK_s) {
      down();
    }
    if (event.key.keysym.sym == SDLK_a) {
      left();
    }
    if (event.key.keysym.sym == SDLK_d) {
      right();
    }
  }
  else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
    switch (event.key.keysym.sym)
    {
    case SDLK_w:
      down();
      break;
    case SDLK_s:
      up();
      break;
    case SDLK_a:
      right();
      break;
    case SDLK_d:
      left();
      break;
    }
  }
}

void Cursor::right() {
  setVelocityX(getVelocityX() + 300.0);
}
void Cursor::left() {
  setVelocityX(getVelocityX() - 300.0);
}
void Cursor::up() {
  setVelocityY(getVelocityY() - 300.0);
}
void Cursor::down() {
  setVelocityY(getVelocityY() + 300.0);
}

void Cursor::stop() {
  setVelocityX(0);
  setVelocityY(0);
}
