#include "application.h"

const std::string Application::kResPath = "../res/";

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

    //Update();
    Render();
  }

  Cleanup();
  return 0;
}

bool Application::Initialize() {
  // start up SDL and make sure it went ok
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    LogSDLError(std::cout, "SDL_Init");
    return false;
  }

  // initialize SDL_image
  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
    LogSDLError(std::cout, "IMG_Init");
    Cleanup();
    return false;
  }

  // set hint
  if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == SDL_FALSE) {
    LogSDLError(std::cout, "SDL_SetHint");
  }

  // setup window
  window_ = SDL_CreateWindow(
    title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    kWindowWidth, kWindowHeight, SDL_WINDOW_SHOWN);
  if (window_ == nullptr) {
    LogSDLError(std::cout, "SDL_CreateWindow");
    Cleanup();
    return false;
  }

  // setup renderer
  renderer_ = SDL_CreateRenderer(
    window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer_ == nullptr) {
    LogSDLError(std::cout, "SDL_CreateRenderer");
    Cleanup();
    return false;
  }

  // load textures
  std::string board_file = kResPath + "board.png";
  std::string x_file = kResPath + "x.png";
  std::string o_file = kResPath + "o.png";
  textures_.push_back(IMG_LoadTexture(renderer_, board_file.c_str()));
  textures_.push_back(IMG_LoadTexture(renderer_, x_file.c_str()));
  textures_.push_back(IMG_LoadTexture(renderer_, o_file.c_str()));
  for (auto texture : textures_) {
    if (texture == nullptr) {
      Cleanup();
      return false;
    }
  }
  return true;
}

void Application::PollEvent(const SDL_Event& event, bool& quit) {
  switch (event.type) {
  case SDL_MOUSEBUTTONDOWN:
    // get mouse position
    int x, y;
    SDL_GetMouseState(&x, &y);

    break;
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
  SDL_RenderClear(renderer_);
  RenderTexture(textures_[0], 0, 0);
  RenderTexture(textures_[1], 0, 0);
  RenderTexture(textures_[1], 130, 0);
  RenderTexture(textures_[1], 260, 0);
  RenderTexture(textures_[1], 0, 130);
  RenderTexture(textures_[1], 130, 130);
  RenderTexture(textures_[1], 260, 130);
  RenderTexture(textures_[1], 0, 260);
  RenderTexture(textures_[1], 130, 260);
  RenderTexture(textures_[1], 260, 260);
  SDL_RenderPresent(renderer_);
}

void Application::RenderTexture(SDL_Texture* texture,
  const int& x, const int& y, int w, int h) {
  // Setup the destination rectangle to be at the position we want
  SDL_Rect rect;
  if (w < 0 || h < 0) {
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
  }
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  SDL_RenderCopy(renderer_, texture, NULL, &rect);
}

void Application::Cleanup() {
  for (auto texture : textures_) {
    if (texture) {
      SDL_DestroyTexture(texture);
    }
  }
  if (renderer_) {
    SDL_DestroyRenderer(renderer_);
  }
  if (window_) {
    SDL_DestroyWindow(window_);
  }

  IMG_Quit();
  SDL_Quit();
}

void Application::LogSDLError(std::ostream& os, const std::string& msg) const {
  os << msg << " error: " << SDL_GetError() << std::endl;
}
