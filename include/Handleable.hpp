#ifndef _STATE_MACHINE_HPP_
#define _STATE_MACHINE_HPP_

#include "api.h"

//#include "Intake.hpp"
//#include "Lift.hpp"
//#include "Tilt.hpp"
//#include "LiftTilt.hpp"

//class Intake::Machine;
//class Lift::Machine;
//class Tilt::Machine;
//class LiftTilt::Machine;

class Handleable{
protected:
  std::function<void(void)> currentState;
public:
  void handle(void);
};

#endif //_STATE_MACHINE_HPP_
