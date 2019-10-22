#ifndef _TILT_MACHINE_HPP_
#define _TILT_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Tilt{
  const int DEADBAND = 20;
  const int DEF_VELOCITY = 60;
  const int SLOW_VELOCITY = 35;
  const int SLOW_SPOT = 00;
  const int BOT_POWER = -4;

  enum State {
    TOWER,
    BOT_INTAKE,
    MID_INTAKE,
    HIGH_INTAKE,
    DROP_STACK,
    STOP,
    CALIBRATE
  };

  class Machine : public Handleable{
    MotorGroup* tiltMotors;
  public:
    Machine(MotorGroup* tiltMotors);

    Poller setState(State state);

    Poller dropStack(void);

    Poller bottom(void);

    Poller calibrate(void);
  };
};

#endif //_TILT_MACHINE_HPP_
