#ifndef _INTAKE_HPP_
#define _INTAKE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Intake {
  const int STOP_POWER = 0;
  const int INTAKE_POWER = 110;
  const int HOLD_POWER = 40;
  const int OUTTAKE_POWER = -70;
  const int TOWER_OUTTAKE_POWER = -70;

  enum State {
    STACK_OUTTAKE = -2,
    OUTTAKE = -1,
    STOP = 0,
    HOLD = 1,
    INTAKE = 2,
  };

  class Machine : public Handleable{
    MotorGroup* intakeMotors;
  public:
    Machine(MotorGroup* intakeMotors);

    Poller setState(State state);
  };
};

#endif //_INTAKE_HPP_
