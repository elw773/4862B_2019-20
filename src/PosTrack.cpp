#include "main.h"


PosTrack::PosTracker::PosTracker( int straightEncPort1, int straightEncPort2, bool straightReversed,
            int sidewaysEncPort1, int sidewaysEncPort2, bool sidewaysReversed,
            int imuPort,
            double straightWheelOffset,
            double sidewaysWheelOffset,
            double straightTicksToInches,
            double sidewaysTicsToInches
          ) :  straightEnc (straightEncPort1, straightEncPort2, straightReversed),  sidewaysEnc (sidewaysEncPort1, sidewaysEncPort2, sidewaysReversed)//, imu(imuPort)
{
  this->straightWheelOffset = straightWheelOffset;
  this->sidewaysWheelOffset = sidewaysWheelOffset;
  this->straightTicksToInches = straightTicksToInches;
  this->sidewaysTicsToInches = sidewaysTicsToInches;
  posCondition.robotPose.x = 0;
  posCondition.robotPose.y = 0;
  posCondition.robotPose.a = 0;
  posCondition.straightDist = getStraightDist();
  posCondition.sidewaysDist = getSidewaysDist();
};

double PosTrack::PosTracker::getStraightDist(void){
  return straightEnc.get_value() * straightTicksToInches;
};

double PosTrack::PosTracker::getSidewaysDist(void){
  return sidewaysEnc.get_value() * sidewaysTicsToInches;
};

Pose* PosTrack::PosTracker::getPose(void){
  return &posCondition.robotPose;
};

void PosTrack::PosTracker::update(void){
  double straightDist = getStraightDist();
  double sidewaysDist = getSidewaysDist();
  double angle = getAngle();


  double straightDelta = straightDist - posCondition.straightDist;
  double sidewaysDelta = sidewaysDist - posCondition.sidewaysDist;
  double angleDelta = angle - posCondition.robotPose.a;

  double averageAngle = angle + (angleDelta  * 0.5);

  double twoSinAoverTwo = 2 * sin(angleDelta * 0.5);

  if(angleDelta != 0){
    localStraightMovement.s = twoSinAoverTwo * ((straightDelta / angleDelta) + straightWheelOffset);
    localSidewaysMovement.s = twoSinAoverTwo * ((sidewaysDelta / angleDelta) + sidewaysWheelOffset);
  } else {
    localStraightMovement.s = straightDelta;
    localSidewaysMovement.s = sidewaysDelta;
  }


  localStraightMovement.a = averageAngle;
  localSidewaysMovement.a = averageAngle - (M_PI * 0.5);

  addPolarToPose(&posCondition.robotPose, localStraightMovement);
  addPolarToPose(&posCondition.robotPose, localSidewaysMovement);

  posCondition.straightDist = straightDist;
  posCondition.sidewaysDist = sidewaysDist;
  posCondition.robotPose.a = angle;
};

void addPolarToPose(Pose* pose, PolarPos polar){
  pose->x += (polar.s * cos(polar.a));

  pose->y += (polar.s * sin(polar.a));
};

double PosTrack::PosTracker::getAngle(void){
  return Robot::drive.getAngle() * 0.0174532925;
};

double calcDist(VectorPos a, VectorPos b){
  return calcDist(a.x, a.y, b.x, b.y);
};

double calcDist(Pose a, VectorPos b){
  return calcDist(a.x, a.y, b.x, b.y);
};

double calcDist(Pose a, Pose b){
  return calcDist(a.x, a.y, b.x, b.y);
};

double calcDist(double xa, double ya, double xb, double yb){
  return sqrt(pow(xb-xa, 2) + pow(yb-ya, 2));
};

double calcAngle(VectorPos a, VectorPos b){
  return calcAngle(a.x, a.y, b.x, b.y);
};

double calcAngle(Pose a, VectorPos b){
  return calcAngle(a.x, a.y, b.x, b.y);
};

double calcAngle(Pose a, Pose b){
  return calcAngle(a.x, a.y, b.x, b.y);
};

double calcAngle(double xa, double ya, double xb, double yb){
  return atan2(yb-ya, xb-xa);
};
