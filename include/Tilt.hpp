#ifndef _TILT_MACHINE_HPP_
#define _TILT_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Tilt{
  const int CALIBRATE_OFFSET = 30;
  const int DEADBAND = 30;
  const int SLOW_VELOCITY = 90;
  const int DEF_VELOCITY = 100;

  const double SLOW_POS = 1500;
  const int BOT_HOLD_POWER = 0;

  const double TOWER_POS = 580;
  const double BOT_INTAKE_POS = 0;
  const double MID_INTAKE_POS = 2500;
  const double HIGH_INTAKE_POS = 4400;
  const double DROP_STACK_POS = 9000;

  enum State {
    TOWER = 5,
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
    MotorGroup* tiltMotors;
    Poller poller;
  public:
    Poller* getPoller(void);
    double stateToPos(State state);

    Machine(MotorGroup* tiltMotors);

    void movePower(int power);
    void setState(State state);

    void calibrate(void);
    void dropStack(void);

    void handle(void);
  };
};

#endif //_TILT_MACHINE_HPP_
