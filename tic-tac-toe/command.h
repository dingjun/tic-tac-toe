#ifndef COMMAND_H_
#define COMMAND_H_

#include "state.h"

class Command {
public:
  virtual ~Command() {}
  virtual void execute(State& state) = 0;
};

#endif  // COMMAND_H_
