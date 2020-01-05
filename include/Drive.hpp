#ifndef _DRIVE_MACHINE_HPP_
#define _DRIVE_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"
#include "Utility.hpp"

namespace Drive {
  const double DEF_RANGE = 0.5;

  enum State{
    POWER,
    DISTANCE,
    DRIVE_TO_POINT,
    ANGLE,
    TURN_TO_POINT
  };

  const double ticksToDistance = 0.0185185;
  const double distanceToTicks = 1/ticksToDistance;
  const double ticksToAngle = 0.098361; //0.1000639
  const double angleToTicks = 1/ticksToAngle;

  class Machine : public Handleable{
    MotorGroup* leftMotors;
    MotorGroup* rightMotors;
    PosTrack::PosTracker* posTracker;
    Poller poller;
  public:
    Poller* getPoller(void);

    Machine(MotorGroup* leftMotors, MotorGroup* rightMotors, PosTrack::PosTracker* posTracker);

    void move(int left, int right);

    void moveDistance(double distance, int velocity, double range = 0.5, int timeout = 200);

    void moveAngle(double angle, int velocity, double range = 5, int timeout = 200);

    void driveToPoint(double xGoal, double yGoal, int velocity, bool stop, int maxCorrection, bool reverse, double range = 0.5);

    void turnToPoint(double xGoal, double yGoal, int velocity, bool stop, double range = degreeToRad(3.0));

    double getDistance();

    double getAngle();
  };
};

#endif //_DRIVE_MACHINE_HPP_
