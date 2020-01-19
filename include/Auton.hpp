#ifndef _AUTON_HPP_
#define _AUTON_HPP_

#include "api.h"

namespace Atn {
  const bool RED = true;
  const bool BLUE = false;

  extern int selectedAuton;
  extern bool deployed;

  void nextAuton(void);
  void prevAuton(void);
  std::string getSelectedAutonName(void);

  void runAuton(void);

  class Auton{
    std::function<void(bool)> func;
    std::string name;
  public:
    std::string getName(void);
    Auton(std::function<void(bool)> func, std::string name);
    void run(bool side);
  };

  void dropStack(void);

  void deploy(void);

  extern std::vector<Auton*> autons;
};

#endif //_AUTON_HPP_
