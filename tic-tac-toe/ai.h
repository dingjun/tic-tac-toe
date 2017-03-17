#ifndef AI_H_
#define AI_H_

#include <vector>
#include "state.h"

struct Node {
  Node(State* s) { state = s; }

  State* state;
  std::vector<Node*> children;
  int value;
};

class AI {
public:
  AI(int level = 9) : level_(level) {}
  ~AI() {}

  void GetNextInput(State& state, int& x, int& y);

private:
  int AlphaBetaPruning(
    Node* node, int depth, int alpha, int beta, bool maximizingPlayer);
  
  int level_;
};

#endif  // AI_H_
