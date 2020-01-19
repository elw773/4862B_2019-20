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

  enum StopType{
    DRIVE_THROUGH,
    SOFT_STOP,
    HARD_STOP
  };

  const double ticksToDistance = 0.0185185;
  const double distanceToTicks = 1/ticksToDistance;
  const double ticksToAngle = 0.098361; //0.1000639
  const double angleToTicks = 1/ticksToAngle;
  Vector calculateLookaheadPoint(Line line, Vector robotPos);

  class Machine : public Handleable{
    MotorGroup* leftMotors;
    MotorGroup* rightMotors;
    PosTrack::PosTracker* posTracker;
    Poller poller;
    double aIntegral;
    bool deccelZone;
  public:
    void reset(void);

    void calibrate(void);

    Poller* getPoller(void);

    Machine(MotorGroup* leftMotors, MotorGroup* rightMotors, PosTrack::PosTracker* posTracker);

    void move(int left, int right);

    void slewVelocity(int left, int right);

    void moveDistance(double distance, int velocity, double range = 0.5, int timeout = 0);

    void moveAngleDeg(double angle, int velocity, double range = 1, int timeout = 0);

    void driveToPointLine(double xGoal, double yGoal, int velocity, StopType stopType, bool reverse, double range = 1);

    void driveToPointLine(double xStart, double yStart, double xGoal, double yGoal, int velocity, StopType stopType, bool reverse, double range = 1);

    void driveToPoint(double xGoal, double yGoal, int velocity, StopType stopType, bool reverse, double range = 1);

    void turnToPoint(double xGoal, double yGoal, int velocity, StopType stopType, double range = degreeToRad(2));

    void turnToAngle(double aGoal, int velocity, StopType stopType, double range = degreeToRad(2));

    double getDistance();

    double getAngle();
  };
};

#endif //_DRIVE_MACHINE_HPP_
