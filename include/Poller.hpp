#ifndef _POLLER_HPP_
#define _POLLER_HPP_

#include "api.h"

#define NOT_IN_TARGET 0

class Poller{
  int timeInTarget = -1;
  std::function<bool(int*)> isDone;

public:
  Poller(void); // poller that is always true

  Poller(int time);

  Poller(std::function<bool(int*)> isDone);

  Poller(std::function<double(void)> value, double target, double range, int timeout);

  Poller(std::function<double(void)> value, double target);

  bool finished(void);
};

#endif //_POLLER_HPP_
