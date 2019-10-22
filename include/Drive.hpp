#ifndef _DRIVE_MACHINE_HPP_
#define _DRIVE_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"

class DriveMachine{
  MotorGroup* leftMotors;
  MotorGroup* rightMotors;
  std::function<void(void)> currentState;
public:
  DriveMachine(MotorGroup* leftMotors, MotorGroup* rightMotors);
  Poller drive(int left, int right);

  void handle(void);
};

#endif //_DRIVE_MACHINE_HPP_
