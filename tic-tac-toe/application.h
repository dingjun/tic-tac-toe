#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "state.h"
#include "ai.h"
#include "unit.h"

class Unit;

class Application {
public:
  Application() : title_("Tic-tac-toe"), window_(nullptr), renderer_(nullptr) {}

  int Execute();

  inline SDL_Renderer* get_renderer() { return renderer_; }
  inline SDL_Texture* get_texture(int id) { return textures_[id]; }

private:
  static const int kWindowWidth = 390;
  static const int kWindowHeight = 390;
  static const int kGridSize = 130;
  static const std::string kResPath;

  bool Initialize();                              // initialize SDL application
  bool PollEvent(const SDL_Event& event, SDL_Point& point, bool& quit);
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

  State state_;
  AI ai_;

  std::string title_;
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  std::vector<SDL_Texture*> textures_;
  std::vector<Unit*> units_;
};

#endif  // APPLICATION_H_
