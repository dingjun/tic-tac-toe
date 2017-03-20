#include "ai.h"
#include <limits>
#include <algorithm>

void AI::GetNextInput(State& state, int& x, int& y) {
  Node* root = new Node(&state);

  const char player = state.get_player();
  bool maximizingPlayer;
  if (player == 'x') {
    maximizingPlayer = true;
  }
  else {
    maximizingPlayer = false;
  }
  
  root->value = AlphaBetaPruning(
    root, level_, kMinInt, kMaxInt, maximizingPlayer);

  // TODO: if multiple children avaliable,
  // choose one has higher probability to win
  for (auto child : root->children) {
    if (child->value == root->value) {
      state.GetPlace(*(child->state), x, y);
      return;
    }
  }
}

int AI::AlphaBetaPruning(
  Node* node, int depth, int alpha, int beta, bool maximizingPlayer) {
  State* state = node->state;
  int value = state->value();
  if (depth == 0 || value != 0 || state->draw()) {
    node->value = value;
    return value;
  }

  // construct children
  for (int y = 0; y < 3; ++y) {
    for (int x = 0; x < 3; ++x) {
      State* next_state = new State(state);
      if (next_state->Place(x, y)) {
        next_state->NextPlayer();
        Node* child = new Node(next_state);
        node->children.push_back(child);
      }
    }
  }

  int best_value;
  if (maximizingPlayer) {
    best_value = kMinInt;
    for (auto child : node->children) {
      best_value = std::max(best_value,
        AlphaBetaPruning(child, depth - 1, alpha, beta, false));
      alpha = std::max(alpha, best_value);
      if (beta <= alpha) {
        break;  // beta cut-off
      }
    }
    node->value = best_value;
    return best_value;
  }
  else {
    best_value = kMaxInt;
    for (auto child : node->children) {
      best_value = std::min(best_value,
        AlphaBetaPruning(child, depth - 1, alpha, beta, true));
      beta = std::min(beta, best_value);
      if (beta <= alpha) {
        break;  // beta cut-off
      }
    }
    node->value = best_value;
    return best_value;
  }
}
