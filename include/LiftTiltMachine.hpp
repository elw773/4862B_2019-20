#ifndef _LIFT_TILT_MACHINE_HPP_
#define _LIFT_TILT_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"

enum LiftTiltStates{
  ALLIANCE_TOWER,
  LOW_TOWER,
  MID_TOWER,
  LOW_ANGLE,
  MID_ANGLE,
  OUTTAKE
};

class LiftTiltMachine{
  MotorGroup* liftMotors;
  MotorGroup* tiltMotors;
  std::function<void(void)> currentState;
  LiftTiltStates state;
public:
  LiftTiltMachine(MotorGroup* liftMotors, MotorGroup* tiltMotors);
  Poller setState(LiftTiltStates state);

  void handle(void);
};

#endif //_LIFT_TILT_MACHINE_HPP_
