#include "application.h"

/*
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren) {
  //SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
  if (texture == nullptr) {
    //logSDLError(std::cout, "LoadTexture");
  }
  return texture;
}
/*
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param rend The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*
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
*
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
  int w, h;
  SDL_QueryTexture(tex, NULL, NULL, &w, &h);
  renderTexture(tex, ren, x, y, w, h);
}*/

int Application::Execute() {
  if (Initialize() == false) {
    return 1;
  }

  SDL_Event event;
  bool quit = false;

  while (quit == false) {
    // poll event
    while (SDL_PollEvent(&event) != 0) {
      PollEvent(event, quit);
    }

    Update();
    Render();
  }

  // clean up
  Cleanup(renderer, window);
  //IMG_Quit();
  //TTF_Quit();
  SDL_Quit();

  return 0;
}

bool Application::Initialize() {
  // start up SDL and make sure it went ok
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    LogSDLError(std::cout, "SDL_Init");
    return false;
  }

  // set hint
  if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == SDL_FALSE) {
    LogSDLError(std::cout, "SDL_SetHint");
  }

  // setup window
  window = SDL_CreateWindow(
    title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    kWindowWidth, kWindowHeight, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    LogSDLError(std::cout, "SDL_CreateWindow");
    SDL_Quit();
    return false;
  }

  // setup renderer
  renderer = SDL_CreateRenderer(
    window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    LogSDLError(std::cout, "SDL_CreateRenderer");
    Cleanup(window);
    SDL_Quit();
    return false;
  }

  return true;
}

void Application::PollEvent(const SDL_Event& event, bool& quit) {
  switch (event.type) {
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      quit = true;
    }
    break;
  case SDL_QUIT:
    quit = true;
    break;
  }
}

void Application::Update() {

  // TODO use gui
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
}

void Application::Render() {
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

void Application::LogSDLError(std::ostream& os, const std::string& msg) const {
  os << msg << " error: " << SDL_GetError() << std::endl;
}

template<typename T, typename... Args>
inline void Application::Cleanup(T* t, Args&&... args) {
  // clean up the first item in the list
  Cleanup(t);
  // recurse to clean up the remaining arguments
  Cleanup(std::forward<Args>(args)...);
}

template<>
inline void Application::Cleanup<SDL_Window>(SDL_Window* window) {
  if (window) {
    SDL_DestroyWindow(window);
  }
}
template<>
inline void Application::Cleanup<SDL_Renderer>(SDL_Renderer* renderer) {
  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }
}
template<>
inline void Application::Cleanup<SDL_Texture>(SDL_Texture* texture) {
  if (texture) {
    SDL_DestroyTexture(texture);
  }
}
template<>
inline void Application::Cleanup<SDL_Surface>(SDL_Surface* surface) {
  if (surface) {
    SDL_FreeSurface(surface);
  }
}
