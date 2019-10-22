#ifndef _LIFT_TILT_MACHINE_HPP_
#define _LIFT_TILT_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace LiftTilt {
  enum State{
    ALLIANCE_TOWER,
    LOW_TOWER,
    MID_TOWER,
    BOT_INTAKE,
    LOW_INTAKE,
    MID_INTAKE,
    DROP_STACK,
    CALIBRATE
  };

  class Machine : public Handleable{
    MotorGroup* liftMotors;
    MotorGroup* tiltMotors;
  public:
    Machine(MotorGroup* liftMotors, MotorGroup* tiltMotors);
    Poller setState(State state);

  };
};

#endif //_LIFT_TILT_MACHINE_HPP_
