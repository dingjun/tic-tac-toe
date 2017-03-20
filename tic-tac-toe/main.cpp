#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "cleanup.h"
#include "state.h"
#include "ai.h"

// Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/*
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message too
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg) {
  os << msg << " error: " << SDL_GetError() << std::endl;
}
/*
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren) {
  SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
  if (texture == nullptr) {
    logSDLError(std::cout, "LoadTexture");
  }
  return texture;
}
/*
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param rend The renderer we want to draw too
* @param x The x coordinate to draw too
* @param y The y coordinate to draw too
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void renderTexture(
  SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
  // Setup the destination rectangle to be at the position we want
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopy(ren, tex, NULL, &dst);
}
/*
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param rend The renderer we want to draw too
* @param x The x coordinate to draw too
* @param y The y coordinate to draw too
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
  int w, h;
  SDL_QueryTexture(tex, NULL, NULL, &w, &h);
  renderTexture(tex, ren, x, y, w, h);
}

int main(int argc, char* argv[]) {
  State state;
  AI ai;

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  // Setup our window and renderer
  SDL_Window *window = SDL_CreateWindow(
    "Tic-tac-toe", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    logSDLError(std::cout, "CreateWindow");
    SDL_Quit();
    return 1;
  }
  SDL_Renderer *renderer = SDL_CreateRenderer(
    window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    logSDLError(std::cout, "CreateRenderer");
    cleanup(window);
    SDL_Quit();
    return 1;
  }

  while (true) {
    state.Print();

    if (state.value() != 0) {
      std::cout << "-- player " << state.get_opponent() << " wins" << std::endl;
      break;
    }
    if (state.draw()) {
      std::cout << "-- Draw" << std::endl;
      break;
    }

    int x;
    int y;
    char player = state.get_player();
    std::cout << "player '" << player << "'> ";
    if (player == 'x') {
      std::cin.clear();
      std::cin >> x >> y;
    }
    else {
      ai.GetNextInput(state, x, y);
      std::cout << x << " " << y << std::endl;
    }

    state.Place(x, y);

    state.NextPlayer();
  }
  
  // Destroy the various items
  cleanup(renderer, window);
  IMG_Quit();
  SDL_Quit();

  system("PAUSE");
  return 0;
}
