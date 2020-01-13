#ifndef _LIFT_MACHINE_HPP_
#define _LIFT_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Lift {
  const int CALIBRATE_OFFSET = 30;
  const int DEADBAND = 60;
  const int DEF_VELOCITY = 90;


  const int DROP_STACK_HOLD_POWER = -5;
  const int INTAKE_HOLD_POWER = 0;

  const double PRE_TWO_GRAB_POS = 300;
  const double LOW_TOWER_POS = 1700;
  const double MID_TOWER_POS = 2300;
  const double INTAKE_POS = 0;
  const double DEPLOY_POS = 500;

  extern int holdPower;

  enum State{
    DEPLOY = 7,
    PRE_TWO_GRAB = 6,
    GRAB_STACK = 5,
    LOW_TOWER = 4,
    MID_TOWER = 3,
    INTAKE = 2,
    DROP_STACK = 1,
    STOP = 0,
    CALIBRATE = -1,
    POWER = -2,
    LIFT_POWER = -2,
    TILT_POWER = -3
  };





  class Machine : public Handleable{
    MotorGroup* liftMotors;
    Poller poller;
  public:
    Poller* getPoller(void);
    double stateToPos(State state);
    Machine(MotorGroup* liftMotors);

    void setState(State state);


    void movePower(int power);

    void calibrate(void);

    void grabStack(void);

    void handle(void);
  };
};

#endif //_LIFT_MACHINE_HPP_
