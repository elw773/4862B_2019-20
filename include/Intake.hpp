#ifndef _INTAKE_HPP_
#define _INTAKE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Intake {

  const int STOP_POWER = 0;
  const int INTAKE_POWER = 110;
  const int GRAB_STACK_POWER = 70;
  const int HOLD_POWER = 30;
  const int OUTTAKE_POWER = -70;
  const int STACK_OUTTAKE_POWER = -100;

  enum State {
    PREP_STACK = -5,
    OUTTAKE_CUBE = -4,
    PREP_CUBE = -3,
    STACK_OUTTAKE = -2,
    OUTTAKE = -1,
    STOP = 0,
    HOLD = 1,
    GRAB_STACK = 2,
    INTAKE = 3
  };



  class Machine : public Handleable{
    MotorGroup* intakeMotors;
    Poller poller;
  public:
    Poller* getPoller(void);

    int stateToPower(State state);
    Machine(MotorGroup* intakeMotors);

    void setState(State state);
  };
};

#endif //_INTAKE_HPP_
