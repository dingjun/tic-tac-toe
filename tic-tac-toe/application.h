#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "state.h"
#include "ai.h"

class Application {
public:
  Application() : title("Tic-tac-toe"), window(nullptr), renderer(nullptr) {}

  int Execute();

private:
  static const int kWindowWidth = 640;
  static const int kWindowHeight = 480;

  bool Initialize();                              // initialize SDL application
  void PollEvent(const SDL_Event& event, bool& quit);     // capture SDL Events
  void Update();
  void Render();

  void LogSDLError(std::ostream& os, const std::string& msg) const;

  /*
  * Recurse through the list of arguments to clean up, cleaning up
  * the first one in the list each iteration.
  */
  template<typename T, typename... Args>
  void Cleanup(T* t, Args&&... args);

  /*
  * These specializations serve to free the passed argument and also provide the
  * base cases for the recursive call above, eg. when args is only a single item
  * one of the specializations below will be called by
  * cleanup(std::forward<Args>(args)...), ending the recursion.
  */
  template<>
  inline void Cleanup<SDL_Window>(SDL_Window *window);
  template<>
  inline void Cleanup<SDL_Renderer>(SDL_Renderer *renderer);
  template<>
  inline void Cleanup<SDL_Texture>(SDL_Texture *texture);
  template<>
  inline void Cleanup<SDL_Surface>(SDL_Surface *surface);

  State state;
  AI ai;
  
  std::string title;
  SDL_Window* window;
  SDL_Renderer* renderer;
};

#endif  // APPLICATION_H_
