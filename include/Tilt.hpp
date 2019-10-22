#ifndef _TILT_MACHINE_HPP_
#define _TILT_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Tilt{
  const int DEADBAND = 20;

  const int BOT_POWER = -4;

  enum State {
    TOWER,
    BOT_INTAKE,
    LOW_INTAKE,
    MID_INTAKE,
    DROP_STACK,
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
