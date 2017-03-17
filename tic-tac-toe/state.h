#ifndef STATE_H_
#define STATE_H_

class State {
public:
  const static int kBoardSize = 3;
  const static int kIdIndex = kBoardSize * kBoardSize;
  const static int kDataLength = kIdIndex + 1 + 1;
  const char* kInitState = "         x";

  State() { Initialize(kInitState); }
  State(State* state) { Initialize(state->data_); }
  ~State() {}

  void Initialize(const char* init_data);
  void Print() const;
  bool Place(const int x, const int y);
  void GetPlace(const State& new_state, int& x_index, int& y_index);
  void NextPlayer();

  int value() const;
  bool draw() const;

  char get_player() const { return data_[kIdIndex]; }
  char get_opponent() const;

  int valid_grid_count() const;

private:
  bool is_line(const int x, const int y);

  bool player_line(char player) const;

  void set_player(char player);

  char data_[kDataLength];
};

#endif  // STATE_H_
