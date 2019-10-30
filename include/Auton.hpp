#ifndef _AUTON_HPP_
#define _AUTON_HPP_

#include "api.h"

namespace Atn {
  const bool RED = true;
  const bool BLUE = false;

  class Auton{
    std::function<void(bool)> func;
    std::string name;
  public:
    Auton(std::function<void(bool)> func, std::string name);
    void run(bool side);
  };

  extern std::vector<Auton> autons;
};

#endif //_AUTON_HPP_
