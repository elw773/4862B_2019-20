#include "main.h"

Drive::Machine::Machine(MotorGroup* leftMotors, MotorGroup* rightMotors, PosTrack::PosTracker* posTracker){
  this->leftMotors = leftMotors;
  this->rightMotors = rightMotors;
  this->posTracker = posTracker;
};

void Drive::Machine::move(int left, int right){
  this->state = POWER;
  this->currentState = [left, right, this](void){
    leftMotors->move(left);
    rightMotors->move(right);
  };
  poller = Poller();
};

void Drive::Machine::moveDistance(double distance, int velocity, double range, int timeout){
  this->state = DISTANCE;
  double tickGoal = distance * distanceToTicks;
  leftMotors->setZeroPosition();
  rightMotors->setZeroPosition();

  this->currentState = [tickGoal, velocity, this](void){
    leftMotors->movePosition(tickGoal, velocity);
    rightMotors->movePosition(tickGoal, velocity);
  };

  poller = Poller(std::bind(&Machine::getDistance, this), distance, range, timeout);
};

void Drive::Machine::moveAngle(double angle, int velocity, double range, int timeout){
  this->state = ANGLE;
  double tickGoal = angle*angleToTicks;
  leftMotors->setZeroPosition();
  rightMotors->setZeroPosition();

  this->currentState = [tickGoal, velocity, this](void){
    rightMotors->movePosition(tickGoal * 0.5, velocity);
    leftMotors->movePosition(tickGoal * -0.5, velocity);
  };

  poller = Poller(std::bind(&Machine::getAngle, this), angle, range, timeout);
};

const double kPsl = 35;
const double kPa = 350;

void Drive::Machine::driveToPoint(double xGoal, double yGoal, int velocity, bool stop, int maxCorrection, bool reverse, double range){
  this->state = DRIVE_TO_POINT;
  poller = Poller(false);
  this->currentState = [xGoal, yGoal, velocity, stop, maxCorrection, range, reverse, this]{

    VectorPos goalPos;
    goalPos.x = xGoal;
    goalPos.y = yGoal;
    Pose robotPose = *posTracker->getPose();
    double slError = calcDist(robotPose, goalPos);

    int leftVel, rightVel;
    if(stop){
      leftVel = limit(slError * kPsl, velocity);
      rightVel = limit(slError * kPsl, velocity);
    } else {
      leftVel = velocity;
      rightVel = velocity;
    }

    double aError = calcAngle(robotPose, goalPos) - robotPose.a;
    if(reverse){
      aError -= M_PI * sign(aError); // add 180 degrees if the robot should go backwards
    }
    aError = clipAngle(aError);


    double straightDist = slError * cos(aError);
    double sidewaysDist = slError * sin(aError);
    double correction = fabs(limit(aError * kPa, maxCorrection));
    double otherCorrection = correction;
    if(fabs(straightDist) > 3 || fabs(sidewaysDist) > 3.5){

      if(fabs(aError) < 1.04719755){ // only turn completely if too far off in angle
        otherCorrection = 0;

      } else {
        leftVel = 0;
        rightVel = 0;
      }

      switch(sign(aError)){
        case 1:
          leftVel -= correction;
          rightVel += otherCorrection;
          break;
        case -1:
          rightVel -= correction;
          leftVel += otherCorrection;
          break;
        default:
          break;
        }
      }

      int dirrection = reverse?-1:1;

      int leftVelDelta = limit(leftVel - leftMotors->getTargetVelocity(), 3);
      leftMotors->moveVelocity((leftMotors->getTargetVelocity() + leftVelDelta) * dirrection);

      int rightVelDelta = limit(rightVel - rightMotors->getTargetVelocity(), 3);
      rightMotors->moveVelocity((rightMotors->getTargetVelocity() + rightVelDelta) * dirrection);


      if((fabs(straightDist) < range || fabs(aError) > (M_PI / 2)) && fabs(sidewaysDist) < 4){ // if close or overshot, stop
        poller.setPoller(true);

        if(stop){
          leftMotors->moveVelocity(0);
          rightMotors->moveVelocity(0);
        }


      }
  };
}


void Drive::Machine::turnToPoint(double xGoal, double yGoal, int velocity, bool stop, double range){
  this->state = TURN_TO_POINT;
  poller = Poller(false);
  this->currentState = [xGoal, yGoal, velocity, stop, range, this]{
    VectorPos goalPos;
    goalPos.x = xGoal;
    goalPos.y = yGoal;
    Pose robotPose = *posTracker->getPose();

    double aError = calcAngle(robotPose, goalPos) - robotPose.a;



  };
};

double Drive::Machine::getDistance(){
  return ((leftMotors->getPosition() + rightMotors->getPosition())/2)*ticksToDistance;
};

double Drive::Machine::getAngle(){
  return (rightMotors->getPosition() - leftMotors->getPosition()) * 0.089;
};
