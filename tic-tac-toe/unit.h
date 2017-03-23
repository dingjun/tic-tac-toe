#ifndef UNIT_H_
#define UNIT_H_

#include <SDL.h>
#include "application.h"

class Unit {
public:
  Unit(SDL_Texture* texture, const SDL_Point& point);
  Unit(SDL_Texture* texture, const SDL_Rect& rect);
    
  void Render(SDL_Renderer* renderer);

private:
  SDL_Texture* texture_;
  SDL_Rect rect_;
};

#endif  // UNIT_H_
