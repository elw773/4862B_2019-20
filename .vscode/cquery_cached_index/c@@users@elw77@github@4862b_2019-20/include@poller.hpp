#ifndef _POLLER_HPP_
#define _POLLER_HPP_

#include "api.h"

enum ActionState {
  STARTED = 0,
  CUSTOM1 = 1, // CUSTOM are for defined points along the action
  CUSTOM2 = 2,
  CUSTOM3 = 3,
  SETTLING = 4,
  DONE = 5
};

class Poller {
  std::function<ActionState()> getState;
  public:
    Poller(std::function<ActionState()> getState);

    Poller(int *value, int settlingRange, int wait, int custom1 = INT32_MAX, int custom2 = INT32_MAX, int custom3 = INT32_MAX);

    ActionState getActionState();

    ActionState wait(ActionState actionState);
};

#endif //_POLLER_HPP_
