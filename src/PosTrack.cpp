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
  currCondition.robotVector.x = 0;
  currCondition.robotVector.y = 0;
  currCondition.robotVector.a = 0;
  currCondition.straightDist = getStraightDist();
  currCondition.sidewaysDist = getSidewaysDist();
};

double PosTrack::PosTracker::getStraightDist(void){
  return straightEnc.get_value() * straightTicksToInches;
};

double PosTrack::PosTracker::getSidewaysDist(void){
  return sidewaysEnc.get_value() * sidewaysTicsToInches;
};

Vector* PosTrack::PosTracker::getVector(void){
  return &currCondition.robotVector;
};

void PosTrack::PosTracker::update(void){
  Vector lastVector = currCondition.robotVector;

  double straightDist = getStraightDist();
  double sidewaysDist = getSidewaysDist();
  double angle = getAngle();


  double straightDelta = straightDist - currCondition.straightDist;
  double sidewaysDelta = sidewaysDist - currCondition.sidewaysDist;
  double angleDelta = angle - currCondition.robotVector.a;

  double averageAngle = angle + (angleDelta  * 0.5);

  double twoSinAoverTwo = 2 * sin(angleDelta * 0.5);

  if(angleDelta != 0){
    localStraightMovement.m = twoSinAoverTwo * ((straightDelta / angleDelta) + straightWheelOffset);
    localSidewaysMovement.m = twoSinAoverTwo * ((sidewaysDelta / angleDelta) + sidewaysWheelOffset);
  } else {
    localStraightMovement.m = straightDelta;
    localSidewaysMovement.m = sidewaysDelta;
  }


  localStraightMovement.phi = averageAngle;
  localSidewaysMovement.phi = averageAngle - (M_PI * 0.5);

  addPolarToVector(&currCondition.robotVector, localStraightMovement);
  addPolarToVector(&currCondition.robotVector, localSidewaysMovement);

  currCondition.straightDist = straightDist;
  currCondition.sidewaysDist = sidewaysDist;
  currCondition.robotVector.a = angle;

  //getVelocity
  long elapsedTime = pros::millis() - lastTime;

  velocity.x = (currCondition.robotVector.x - lastVector.x) * 1000.0 / elapsedTime;
  velocity.y = (currCondition.robotVector.y - lastVector.y) * 1000.0 / elapsedTime;
  velocity.a = (currCondition.robotVector.a - lastVector.a) * 1000.0 / elapsedTime;

  lastTime = pros::millis();
};

Polar vectorToPolar(Vector vector){
  Polar polar;
  polar.m = sqrt(vector.x * vector.x + vector.y * vector.y);
  polar.phi = atan2(vector.y, vector.x);
  polar.a = vector.a;
  return polar;
};

Vector polarToVector(Polar polar){
  Vector vector;
  vector.x = polar.m * cos(polar.phi);
	vector.y = polar.m * sin(polar.phi);
  vector.a = polar.a;
  return vector;
};

Vector* PosTrack::PosTracker::getVelocity(void){
  return &velocity;
};

void addPolarToVector(Vector* Vector, Polar polar){
  Vector->x += (polar.m * cos(polar.phi));

  Vector->y += (polar.m * sin(polar.phi));
};

double PosTrack::PosTracker::getAngle(void){
  return Robot::drive.getAngle() * 0.0174532925;
};

double calcDist(Vector a, Vector b){
  return calcDist(a.x, a.y, b.x, b.y);
};

double calcDist(double xa, double ya, double xb, double yb){
  return sqrt(pow(xb-xa, 2) + pow(yb-ya, 2));
};

double calcAngle(Vector a, Vector b){
  return calcAngle(a.x, a.y, b.x, b.y);
};

double calcAngle(double xa, double ya, double xb, double yb){
  return atan2(yb-ya, xb-xa);
};

double calcAngle(Line line){
  return calcAngle(line.a, line.b);
};

double calcSlope(Line line){
  double rise =(line.b.y - line.a.y);
  double run = (line.b.x - line.a.x);
  if(run == 0){
    run= 0.00000001;
  }
  double slope = rise / run;

  return slope;
};

double calcSlope(double angle){
  return tan(angle);
};
