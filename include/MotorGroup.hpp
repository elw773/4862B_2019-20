#ifndef _MOTOR_GROUP_HPP_
#define _MOTOR_GROUP_HPP_

#define GEARSET_100_RPM MOTOR_GEARSET_06
#define GEARSET_200_RPM MOTOR_GEARSET_18
#define GEARSET_600_RPM MOTOR_GEARSET_36

#include "api.h"

class MotorGroup{
  std::vector<pros::Motor*> motors;
  int slew;

  public:
    MotorGroup(std::vector<pros::Motor*> motors, int slew = INT32_MAX);

    void move(int power); // sets motor voltage from -127 to 127

    void movePosition(double position, int velocity); // moves motor to position in degrees

    void moveVelocity(int velocity); // sets motor velocity in rpm

    void setZeroPosition(void);

    double getTargetPosition(void);

    int getTargetVelocity(void);

    int getVelocity(void);
};

#endif //_MOTOR_GROUP_HPP_
