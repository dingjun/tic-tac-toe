#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "state.h"
#include "ai.h"

class Application {
public:
  Application() : title("Tic-tac-toe"), window(nullptr), renderer(nullptr) {}

  int Execute();

private:
  static const int kWindowWidth = 390;
  static const int kWindowHeight = 390;
  static const std::string kResPath;

  bool Initialize();                              // initialize SDL application
  void PollEvent(const SDL_Event& event, bool& quit);     // capture SDL Events
  void Update();
  void Render();

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
  void RenderTexture(SDL_Texture* texture,
    const int& x, const int& y, int w = -1, int h = -1);

  void Cleanup();

  /*
  * Log an SDL error with some error message to the output stream of our choice
  * @param os The output stream to write the message to
  * @param msg The error message to write, format will be msg error: SDL_GetError()
  */
  void LogSDLError(std::ostream& os, const std::string& msg) const;

  State state;
  AI ai;

  std::string title;
  SDL_Window* window;
  SDL_Renderer* renderer;
  std::vector<SDL_Texture*> textures;
};

#endif  // APPLICATION_H_
