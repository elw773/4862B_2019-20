#ifndef _LIFT_TILT_MACHINE_HPP_
#define _LIFT_TILT_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Lift.hpp"
#include "Tilt.hpp"
#include "Handleable.hpp"

namespace LiftTilt {
  enum State{
    ALLIANCE_TOWER,
    LOW_TOWER,
    MID_TOWER,
    BOT_INTAKE,
    MID_INTAKE,
    HIGH_INTAKE,
    STOP,
    CALIBRATE
  };

  class Machine : public Handleable{
    Tilt::Machine* tilt;
    Lift::Machine* lift;
  public:
    Machine(Tilt::Machine* tilt, Lift::Machine* lift);

    Poller setState(State state);
  };
};

#endif //_LIFT_TILT_MACHINE_HPP_
