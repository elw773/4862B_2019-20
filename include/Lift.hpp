#ifndef _LIFT_MACHINE_HPP_
#define _LIFT_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Lift {
  const int DEADBAND = 20;
  const int DEF_VELOCITY = 90;


  const int DROP_STACK_HOLD_POWER = -5;
  const int INTAKE_HOLD_POWER = -1;

  const double PRE_TWO_GRAB_POS = 700;
  const double LOW_TOWER_POS = 1900;
  const double MID_TOWER_POS = 2450;
  const double INTAKE_POS = 0;

  extern int holdPower;

  enum State{
    PRE_TWO_GRAB = 6,
    GRAB_STACK = 5,
    LOW_TOWER = 4,
    MID_TOWER = 3,
    INTAKE = 2,
    DROP_STACK = 1,
    STOP = 0,
    CALIBRATE = -1,
    POWER = -2,
    LIFT_POWER = -2
  };





  class Machine : public Handleable{
    MotorGroup* liftMotors;
  public:
    double stateToPos(State state);
    Machine(MotorGroup* liftMotors);

    Poller setState(State state);


    Poller movePower(int power);

    Poller calibrate(void);

    Poller grabStack(void);

    void handle(void);
  };
};

#endif //_LIFT_MACHINE_HPP_
