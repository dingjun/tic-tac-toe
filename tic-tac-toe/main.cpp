#include <iostream>
#include "state.h"
#include "ai.h"

int main(int argc, char* argv[]) {
  State state;
  AI ai;

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
  system("PAUSE");
  return 0;
}
