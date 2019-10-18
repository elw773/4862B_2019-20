#ifndef _INTAKE_MACHINE_HPP_
#define _INTAKE_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"


enum IntakeState {
  STOP = 0,
  INTAKE = 110,
  HOLD = 40,
  OUTTAKE = -70,
  STACK_OUTTAKE = -100,
  TOWER_OUTTAKE = -70
};

class IntakeMachine{
  MotorGroup* intakeMotors;
  std::function<void(void)> currentState;
  IntakeState state;
public:
  IntakeMachine(MotorGroup* intakeMotors);
  Poller setState(IntakeState state);
  IntakeState getState(void);

  void handle(void);
};

#endif //_INTAKE_MACHINE_HPP_
