#ifndef _TILT_MACHINE_HPP_
#define _TILT_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Tilt{
  const int DEADBAND = 20;
  const int SLOW_VELOCITY = 50;
  const int DEF_VELOCITY = 90;

  const double SLOW_POS = 1500;
  const int BOT_HOLD_POWER = 0;

  const double TOWER_POS = 550;
  const double BOT_INTAKE_POS = 0;
  const double MID_INTAKE_POS = 800;
  const double HIGH_INTAKE_POS = 1500;
  const double DROP_STACK_POS = 4300;

  enum State {
    TOWER = 5,
    DROP_STACK = 4,
    HIGH_INTAKE = 3,
    MID_INTAKE = 2,
    BOT_INTAKE = 1,
    STOP = 0,
    CALIBRATE = -1,
    LIFT_POWER = -2
  };


  class Machine : public Handleable{
    MotorGroup* tiltMotors;
  public:
    double stateToPos(State state);

    Machine(MotorGroup* tiltMotors);

    Poller setState(State state);

    Poller calibrate(void);
    Poller dropStack(void);

    void handle(void);
  };
};

#endif //_TILT_MACHINE_HPP_
