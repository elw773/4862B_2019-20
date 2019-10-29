#include "main.h"

Drive::Machine::Machine(MotorGroup* leftMotors, MotorGroup* rightMotors){
  this->leftMotors = leftMotors;
  this->rightMotors = rightMotors;
};

Poller Drive::Machine::move(int left, int right){
  this->state = POWER;
  this->currentState = [left, right, this](void){
    leftMotors->move(left);
    rightMotors->move(right);
  };
  return Poller();
};
