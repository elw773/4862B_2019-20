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

Poller Drive::Machine::moveDistance(double distance, int velocity, double range, int timeout){
  this->state = DISTANCE;
  double tickGoal = distance * distanceToTicks;
  leftMotors->setZeroPosition();
  rightMotors->setZeroPosition();

  this->currentState = [tickGoal, velocity, this](void){
    leftMotors->movePosition(tickGoal, velocity);
    rightMotors->movePosition(tickGoal, velocity);
  };

  return Poller(std::bind(&Machine::getDistance, this), distance, range, timeout);
};

Poller Drive::Machine::moveAngle(double angle, int velocity, double range, int timeout){
  this->state = ANGLE;
  double tickGoal = angle*angleToTicks;
  leftMotors->setZeroPosition();
  rightMotors->setZeroPosition();

  this->currentState = [tickGoal, velocity, this](void){
    rightMotors->movePosition(tickGoal * 0.5, velocity);
    leftMotors->movePosition(tickGoal * -0.5, velocity);
  };

  return Poller(std::bind(&Machine::getAngle, this), angle, range, timeout);
};

double Drive::Machine::getDistance(){
  return ((leftMotors->getPosition() + rightMotors->getPosition())/2)*ticksToDistance;
};

double Drive::Machine::getAngle(){
  return (rightMotors->getPosition() - leftMotors->getPosition()) * ticksToAngle;
};
