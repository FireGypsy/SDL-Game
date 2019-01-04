#include "drawable.h"
#include "image.h"
#include "ioMod.h"
#include "viewport.h"
#include "renderContext.h"

Image::Image( SDL_Surface* surf) : 
  renderer(RenderContext::getInstance()->getRenderer()),
  surface( surf ),
  texture( nullptr ),
  view{0,0,surf->w,surf->h}
{ 
  regenerateTexture();
}

Image::Image( const Image& image ) :
  renderer(image.renderer),
  surface(image.surface), 
  texture(image.texture),
  view(image.view)
{ }


Image& Image::operator=(const Image& rhs) {
  renderer = rhs.renderer;
  texture = rhs.texture;
  view = rhs.view;
  return *this;
}

void Image::regenerateTexture(){
  if(texture != nullptr) SDL_DestroyTexture(texture);
  RenderContext* renderContext  = RenderContext::getInstance();
  texture = SDL_CreateTextureFromSurface(renderContext->getRenderer(), surface);
}

void Image::draw(int x, int y) const {
  draw(x,y,1.0f);
}

void Image::draw(int x, int y, float scale) const {
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  int tempHeight = scale*view.h;
  int tempWidth =  scale*view.w;
  SDL_Rect dest =  {x, y, tempWidth, tempHeight};
  SDL_RenderCopy(renderer, texture, &view, &dest);
}

void Image::draw(int sx, int sy, int dx, int dy) const {
  SDL_Rect src = { sx, sy, view.w, view.h };    
  SDL_Rect dst = { dx, dy, getWidth(), getHeight() };
  SDL_RenderCopy(renderer, texture, &src, &dst);
}

//TODO fix this
void Image::draw(int x, int y, SDL_Rect* clip, double angle, SDL_RendererFlip flip) const {
  //Set rendering space and render to screen
  SDL_Rect renderQuad = { x, y, getWidth(), getHeight() };
  SDL_Point* center = NULL;
  flip = SDL_FLIP_NONE;
  //Set clip rendering dimensions
  if (clip != NULL)
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  //Render to screen
  SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}

Image* Image::crop(SDL_Rect sub)const{
  if(sub.x+sub.w > view.w
  || sub.y+sub.h > view.h){
    std::cerr << "Attempted to crop image with invalid geometry."
              << std::endl
              << "(0,0 + "<<view.w << "x"<<view.h<<")"
              << " --> "
              << "("<<sub.x<<","<<sub.y<<" + "<<sub.w << "x"<<sub.h<<")"
              << std::endl;
    return nullptr;
  }

  Image* cloned = new Image(*this);
  cloned->view = sub;

  return cloned;
}

