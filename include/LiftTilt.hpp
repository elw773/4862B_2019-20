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
    PRE_TWO_GRAB = 8,
    GRAB_STACK = 7,
    LOW_TOWER = 6,
    MID_TOWER = 5,
    DROP_STACK = 4,
    HIGH_INTAKE = 3,
    MID_INTAKE = 2,
    BOT_INTAKE = 1,
    STOP = 0,
    CALIBRATE = -1,
    LIFT_POWER = -2,
    TILT_POWER = -3
  };

  class Machine : public Handleable{
    Tilt::Machine* tilt;
    Lift::Machine* lift;
    Poller poller;
  public:
    Poller* getPoller(void);
    Machine(Tilt::Machine* tilt, Lift::Machine* lift);

    void setState(State state);

    void handle(void);
  };
};

#endif //_LIFT_TILT_MACHINE_HPP_
