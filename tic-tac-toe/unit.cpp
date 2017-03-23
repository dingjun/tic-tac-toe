#include "unit.h"

Unit::Unit(SDL_Texture* texture, const SDL_Point& point) {
  SDL_Rect rect;
  rect.x = point.x;
  rect.y = point.y;
  SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

  texture_ = texture;
  rect_ = rect;
}

Unit::Unit(SDL_Texture* texture, const SDL_Rect& rect)
  : texture_(texture), rect_(rect) {
}

void Unit::Render(SDL_Renderer* renderer) {
  SDL_RenderCopy(renderer, texture_, NULL, &rect_);
}
