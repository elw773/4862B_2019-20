#include "main.h"

Intake::Machine::Machine(MotorGroup* intakeMotors){
  this->intakeMotors = intakeMotors;
};

Poller Intake::Machine::setState(State state){
  int power = stateToPower(state);
  this->state = state;

  this->currentState = [power, this](void){
    intakeMotors->move(power);
  };
  return Poller();
}

int Intake::stateToPower(State state){
  switch(state){
    case STACK_OUTTAKE: return STACK_OUTTAKE_POWER;
    case OUTTAKE: return OUTTAKE_POWER;
    case STOP: return STOP_POWER;
    case HOLD: return HOLD_POWER;
    case INTAKE: return INTAKE_POWER;
  }
}
