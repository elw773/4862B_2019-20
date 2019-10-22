#ifndef _LIFT_MACHINE_HPP_
#define _LIFT_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Lift {
  const int DEADBAND = 20;
  const int DROP_STACK_POWER = -15;
  const int INTAKE_POWER = -4;
  const int DEF_VELOCITY = 60;

  enum State{
    ALLIANCE_TOWER,
    LOW_TOWER,
    MID_TOWER,
    INTAKE,
    DROP_STACK,
    CALIBRATE
  };

  class Machine : public Handleable{
    MotorGroup* liftMotors;
  public:
    Machine(MotorGroup* liftMotors);

    Poller setState(State state);

    Poller moveAndHold(double position, int holdPower);

    Poller calibrate(void);
  };
};

#endif //_LIFT_TILT_MACHINE_HPP_
