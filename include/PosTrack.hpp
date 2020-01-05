#ifndef _POS_TRACK_HPP_
#define _POS_TRACK_HPP_

#include "api.h"

struct PolarPos{
  double s;
  double a;
};

struct VectorPos{
  double x;
  double y;
};

struct Pose{
  double x;
  double y;
  double a;
};

namespace PosTrack{
    struct Condition{
      Pose robotPose;
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

    Condition posCondition;

    PolarPos localStraightMovement;
    PolarPos localSidewaysMovement;
  public:
    PosTracker( int straightEncPort1, int straightEncPort2, bool straightReversed,
                int sidewaysEncPort1, int sidewaysEncPort2, bool sidewaysReversed,
                int imuPort,
                double straightWheelOffset,
                double sidewaysWheelOffset,
                double straightTicksToInches,
                double sidewaysTicsToInches
              );

    double getAngle(void); // returns the angle of the robot in radians

    double getStraightDist(void);

    double getSidewaysDist(void);

    Pose* getPose(void);

    void update(void); // updates the position of the robot

    void calibrate(void);
  };
}

double calcDist(VectorPos a, VectorPos b);

double calcDist(Pose a, VectorPos b);

double calcDist(Pose a, Pose b);

double calcDist(double xa, double ya, double xb, double yb);

double calcAngle(VectorPos a, VectorPos b);

double calcAngle(Pose a, VectorPos b);

double calcAngle(Pose a, Pose b);

double calcAngle(double xa, double ya, double xb, double yb);


void addPolarToPose(Pose* pose, PolarPos polar);

Pose vectorPosToPose(VectorPos pos, double angle);

#endif //_POS_TRACK_HPP_
