#ifndef _LIFT_MACHINE_HPP_
#define _LIFT_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Lift {
  const int DEADBAND = 20;
  const int DEF_VELOCITY = 60;

  const int DROP_STACK_HOLD_POWER = -15;
  const int INTAKE_HOLD_POWER = -4;

  const int ALLIANCE_TOWER_POS = 1850;
  const int LOW_TOWER_POS = 2100;
  const int MID_TOWER_POS = 2850;
  const int INTAKE_POS = 0;

  enum State{
    ALLIANCE_TOWER = 5,
    LOW_TOWER = 4,
    MID_TOWER = 3,
    INTAKE = 2,
    DROP_STACK = 1,
    STOP = 0,
    CALIBRATE = -1
  };

  int stateToPos(State state);

  class Machine : public Handleable{
    MotorGroup* liftMotors;
  public:
    Machine(MotorGroup* liftMotors);

    Poller setState(State state);

    Poller moveAndHold(double position, int holdPower);

    Poller calibrate(void);
  };
};

#endif //_LIFT_MACHINE_HPP_
