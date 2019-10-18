#include "main.h"

LiftTiltMachine::LiftTiltMachine(MotorGroup* liftMotors, MotorGroup* tiltMotors){
  this->liftMotors = liftMotors;
  this->tiltMotors = tiltMotors;
};

Poller LiftTiltMachine::setState(LiftTiltStates state){
  switch(state){

  }
  return Poller();
};

void LiftTiltMachine::handle(void){
  this->currentState();
};
