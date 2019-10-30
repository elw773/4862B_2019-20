#ifndef _MOTOR_GROUP_HPP_
#define _MOTOR_GROUP_HPP_

#define GEARSET_600_RPM pros::E_MOTOR_GEARSET_06
#define GEARSET_200_RPM pros::E_MOTOR_GEARSET_18
#define GEARSET_100_RPM pros::E_MOTOR_GEARSET_36

#include "api.h"

#include "Poller.hpp"

class MotorGroup{
  std::vector<pros::Motor*> motors;
  int slew;

public:
  MotorGroup(std::vector<pros::Motor*> motors, int slew = INT32_MAX);

  Poller move(int power); // sets motor voltage from -127 to 127

  Poller movePosition(int position, int velocity, int range = 10, int timeout = 0); // moves motor to position in degrees

  Poller moveVelocity(int velocity, int range = 10); // sets motor velocity in rpm

  void setZeroPosition(void);

  int getTargetPosition(void);

  int getPosition(void);

  int getTargetVelocity(void);

  int getVelocity(void);
};

#endif //_MOTOR_GROUP_HPP_
