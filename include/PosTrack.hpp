#ifndef _POS_TRACK_HPP_
#define _POS_TRACK_HPP_

#include "api.h"

struct Polar{
  double m;
  double phi;
  double a;
};

struct Vector{
  double x;
  double y;
  double a;
};

struct Line{
  Vector a;
  Vector b;
};

namespace PosTrack{
    struct Condition{
      Vector robotVector;
      double straightDist;
      double sidewaysDist;
    };

  class PosTracker{
    //pros::Imu imu;
    pros::ADIEncoder straightEnc; // front sideways encoder
    pros::ADIEncoder sidewaysEnc; // front sideways encoder
    /* declare the imu*/
    double straightWheelOffset;
    double sidewaysWheelOffset;

    double straightTicksToInches;
    double sidewaysTicsToInches;

    Condition currCondition;

    Polar localStraightMovement;
    Polar localSidewaysMovement;

    Vector velocity;
    long lastTime;

    double startAngle;
    pros::Imu imu;
  public:
    PosTracker( int straightEncPort1, int straightEncPort2, bool straightReversed,
                int sidewaysEncPort1, int sidewaysEncPort2, bool sidewaysReversed,
                int imuPort,
                double straightWheelOffset,
                double sidewaysWheelOffset,
                double straightTicksToInches,
                double sidewaysTicsToInches
              );

    void reset(void);

    double getAngle(void); // returns the angle of the robot in radians

    double getStraightDist(void);

    double getSidewaysDist(void);

    Vector* getVector(void);

    Vector* getVelocity(void);

    void update(void); // updates the position of the robot

    void calibrate(void);
  };
}

Vector polarToVector(Polar polar);

Polar vectorToPolar(Vector vector);

double calcDist(Vector a, Vector b);

double calcDist(double xa, double ya, double xb, double yb);

double calcAngle(Vector a, Vector b);

double calcAngle(double xa, double ya, double xb, double yb);

double calcAngle(Line line);

double calcSlope(Line line);

double calcSlope(double angle);

void addPolarToVector(Vector* Vector, Polar polar);

#endif //_POS_TRACK_HPP_
