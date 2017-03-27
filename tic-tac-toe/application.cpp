#include "application.h"

const std::string Application::kResPath = "../res/";

int Application::Execute() {
  if (Initialize() == false) {
    return 1;
  }

  // create board
  SDL_Point point = { 0, 0 };
  Unit* board = new Unit(textures_[0], point);
  units_.push_back(board);

  bool quit = false;
  while (quit == false) {
    char player = state_.get_player();

    // poll event
    SDL_Event event;
    SDL_Point point;
    bool input = false;
    while (SDL_PollEvent(&event) != 0) {
      input = PollEvent(event, point, quit);
    }

    if (player == 'o') {
      state_.Print();
      ai_.GetNextInput(state_, point.x, point.y);
      point.x *= kGridSize;
      point.y *= kGridSize;
      input = true;
    }

    if (input == true) {
      // add unit
      SDL_Texture* texture;
      if (player == 'x') {
        texture = textures_[1];
      }
      else {
        texture = textures_[2];
      }
      units_.push_back(new Unit(texture, point));

      state_.Place(point.x / kGridSize, point.y / kGridSize);
      state_.NextPlayer();
    }
    
    if (state_.value() != 0) {
      std::cout << "-- player " << state_.get_opponent() << " wins" << std::endl;
      quit = true;
    }
    if (state_.draw()) {
      std::cout << "-- Draw" << std::endl;
      quit = true;
    }

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

bool Application::PollEvent(const SDL_Event& event, SDL_Point& point, bool& quit) {
  switch (event.type) {
  case SDL_MOUSEBUTTONDOWN:
    // get mouse position
    SDL_GetMouseState(&point.x, &point.y);
    point.x -= (point.x % kGridSize);
    point.y -= (point.y % kGridSize);

    return true;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      quit = true;
    }
    return false;
  case SDL_QUIT:
    quit = true;
    return false;
  }
}

void Application::Update() {

  while (true) {
    state_.Print();

    if (state_.value() != 0) {
      std::cout << "-- player " << state_.get_opponent() << " wins" << std::endl;
      break;
    }
    if (state_.draw()) {
      std::cout << "-- Draw" << std::endl;
      break;
    }

    int x;
    int y;
    char player = state_.get_player();
    std::cout << "player '" << player << "'> ";
    if (player == 'x') {
      std::cin.clear();
      std::cin >> x >> y;
    }
    else {
      ai_.GetNextInput(state_, x, y);
      std::cout << x << " " << y << std::endl;
    }

    state_.Place(x, y);

    state_.NextPlayer();
  }
}

void Application::Render() {
  SDL_RenderClear(renderer_);
  for (auto unit : units_) {
    unit->Render(renderer_);
  }
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
