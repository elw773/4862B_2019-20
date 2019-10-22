#ifndef _INTAKE_HPP_
#define _INTAKE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Intake {
  enum State {
    STOP = 0,
    INTAKE = 110,
    HOLD = 40,
    OUTTAKE = -70,
    STACK_OUTTAKE = -100,
    TOWER_OUTTAKE = -70
  };

  class Machine : public Handleable{
    MotorGroup* intakeMotors;
  public:
    Machine(MotorGroup* intakeMotors);

    Poller setState(State state);
  };
};

#endif //_INTAKE_HPP_
