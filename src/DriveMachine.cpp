#include "main.h"

DriveMachine::DriveMachine(MotorGroup* leftMotors, MotorGroup* rightMotors){
  this->leftMotors = leftMotors;
  this->rightMotors = rightMotors;
};

Poller DriveMachine::drive(int left, int right){
  this->currentState = [left, right, this](void){
    leftMotors->move(left);
    rightMotors->move(right);
  };
  return Poller();
};

void DriveMachine::handle(void){
  this->currentState();
};
