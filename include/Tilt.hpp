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

  const int SLOW_POS = 00;
  const int BOT_HOLD_POWER = -4;

  const int TOWER_POS = 00;
  const int BOT_INTAKE_POS = 00;
  const int MID_INTAKE_POS = 00;
  const int HIGH_INTAKE_POS = 00;
  const int DROP_STACK_POS = 00;

  enum State {
    TOWER = 5,
    DROP_STACK = 4,
    HIGH_INTAKE = 3,
    MID_INTAKE = 2,
    BOT_INTAKE = 1,
    STOP = 0,
    CALIBRATE = -1
  };

  int stateToPos(State state);

  class Machine : public Handleable{
    MotorGroup* tiltMotors;
  public:
    Machine(MotorGroup* tiltMotors);

    Poller setState(State state);

    Poller calibrate(void);

    void handle(void);
  };
};

#endif //_TILT_MACHINE_HPP_
