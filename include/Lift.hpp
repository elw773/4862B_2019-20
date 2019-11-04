#ifndef _LIFT_MACHINE_HPP_
#define _LIFT_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Lift {
  const int DEADBAND = 20;
  const int DEF_VELOCITY = 90;

  const int DROP_STACK_HOLD_POWER = -15;
  const int INTAKE_HOLD_POWER = -4;

  const int LOW_TOWER_POS = 1900;
  const int MID_TOWER_POS = 2450;
  const int INTAKE_POS = 0;

  extern int holdPower;

  enum State{
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

    Poller calibrate(void);

    void handle(void);
  };
};

#endif //_LIFT_MACHINE_HPP_
