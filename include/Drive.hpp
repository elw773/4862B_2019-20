#ifndef _DRIVE_MACHINE_HPP_
#define _DRIVE_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Drive {
  const double DEF_RANGE = 0.5;

  enum State{
    POWER,
    DISTANCE,
    ANGLE
  };

  const double ticksToDistance = 0.0185185;
  const double distanceToTicks = 1/ticksToDistance;
  const double ticksToAngle = 0.098361;
  const double angleToTicks = 1/ticksToAngle;

  class Machine : public Handleable{
    MotorGroup* leftMotors;
    MotorGroup* rightMotors;
  public:
    Machine(MotorGroup* leftMotors, MotorGroup* rightMotors);

    Poller move(int left, int right);

    Poller moveDistance(double distance, int velocity, double range = 0.5, int timeout = 200);

    Poller moveAngle(double angle, int velocity, double range = 5, int timeout = 200);

    double getDistance();

    double getAngle();
  };
};

#endif //_DRIVE_MACHINE_HPP_
