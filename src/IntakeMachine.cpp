#include "main.h"

#define HOLD_POWER 40
#define INTAKE_POWER 110
#define OUTTAKE_POWER -70
#define STACK_OUTTAKE_POWER -100
#define TOWER_OUTTAKE_POWER -70

IntakeMachine::IntakeMachine(MotorGroup* intakeMotors){
  this->intakeMotors = intakeMotors;
};

Poller IntakeMachine::setState(IntakeState state){
  this->state = state;
  int power = state;
  this->currentState = [power, this](void){
    intakeMotors->move(power);
  };
  return Poller();
}

IntakeState IntakeMachine::getState(){
  return this->state;
}

void IntakeMachine::handle(void){
  this->currentState();
}
