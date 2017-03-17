#include "state.h"
#include <cstring>
#include <iostream>

void State::Initialize(const char* init_data) {
  strcpy_s(data_, kDataLength, init_data);
}

void State::Print() const {
  static const char kHRuler[] = "  0 1 2";
  std::cout << kHRuler << "\n";
  int index = 0;
  for (int y = 0; y < kBoardSize; ++y) {
    std::cout << y << " ";
    for (int x = 0; x < kBoardSize; ++x) {
      std::cout << data_[index++] << " ";
    }
    std::cout << y << "\n";
  }
  std::cout << kHRuler << std::endl;
}

int State::value() const {
  char player = get_player();
  char opponent = get_opponent();

  if (player_line('x')) {
    return 1;
  }
  else if (player_line('o')) {
    return -1;
  }
  else {
    return 0;
  }
}

bool State::draw() const {
  return valid_grid_count() == 0;
}

bool State::Place(const int x, const int y) {
  int index = x + y * kBoardSize;
  if (data_[index] != ' ') {
    return false;
  }
  data_[index] = get_player();
  return true;
}

void State::GetPlace(const State& new_state, int& x_index, int& y_index) {
  int index = 0;
  for (int y = 0; y < 3; ++y) {
    for (int x = 0; x < 3; ++x) {
      if (data_[index] != new_state.data_[index]) {
        x_index = x;
        y_index = y;
        return;
      }
      ++index;
    }
  }
}

void State::NextPlayer() {
  set_player(get_opponent());
}

char State::get_opponent() const {
  char player = get_player();
  if (player == 'o') {
    return 'x';
  }
  else {
    return 'o';
  }
}

int State::valid_grid_count() const {
  int count = 0;
  for (int i = 0; i < kIdIndex; ++i) {
    if (data_[i] == ' ') {
      ++count;
    }
  }
  return count;
}

bool State::is_line(const int x, const int y) {
  // TODO
  return false;
}

bool State::player_line(char player) const {
  if (player == data_[0] && player == data_[1] && player == data_[2]
    || player == data_[3] && player == data_[4] && player == data_[5]
    || player == data_[6] && player == data_[7] && player == data_[8]
    || player == data_[0] && player == data_[3] && player == data_[6]
    || player == data_[1] && player == data_[4] && player == data_[7]
    || player == data_[2] && player == data_[5] && player == data_[8]
    || player == data_[0] && player == data_[4] && player == data_[8]
    || player == data_[2] && player == data_[4] && player == data_[6]) {
    return true;
  }
  return false;
}

void State::set_player(char player) {
  data_[kIdIndex] = player;
}
