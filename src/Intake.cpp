#include "main.h"

Intake::Machine::Machine(MotorGroup* intakeMotors){
  this->intakeMotors = intakeMotors;
};

Poller Intake::Machine::setState(State state){
  int power = state;
  this->currentState = [power, this](void){
    intakeMotors->move(power);
  };
  return Poller();
}
