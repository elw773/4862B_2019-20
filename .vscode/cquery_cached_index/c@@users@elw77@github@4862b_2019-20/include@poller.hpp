#ifndef _POLLER_HPP_
#define _POLLER_HPP_

#include "api.h"

#define NOT_IN_TARGET 0

class Poller{
  int timeInTarget = -1;
  std::function<bool(int*)> isDone;

public:
  Poller(void); // poller that is always true

  Poller(bool b);

  void setPoller(bool b);

  Poller(int time);

  Poller(std::function<bool(int*)> isDone);

  Poller(std::function<bool(void)> isDone);

  Poller(std::function<int(void)> value, int target, int range, int timeout = 0);

  Poller(std::function<int(void)> value, int target);

  Poller(Poller* a, Poller* b);

  bool finished(void);
};

#endif //_POLLER_HPP_
