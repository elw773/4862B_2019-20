#include "main.h"

const double aIntegralLimit = 40;
const double aErrorIntegralRange = degreeToRad(30);
const double kIa = 3;
const double kPa = 200;
const double kPsl = 25;
const double noTurnDist = 7;


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




void Drive::Machine::driveToPoint(double xGoal, double yGoal, int velocity, StopType stopType, int maxCorrection, bool reverse, double range){
  this->state = DRIVE_TO_POINT;
  poller = Poller(false);
  this->currentState = [xGoal, yGoal, velocity, stopType, maxCorrection, range, reverse, this]{

    Vector goalPos;
    goalPos.x = xGoal;
    goalPos.y = yGoal;
    Vector robotVector = *posTracker->getVector();
    double slError = calcDist(robotVector, goalPos);

    int leftVel, rightVel;
    if(stopType == StopType::DRIVE_THROUGH){
      leftVel = velocity;
      rightVel = velocity;
    } else {
      leftVel = limit(slError * kPsl, velocity);
      rightVel = limit(slError * kPsl, velocity);
    }

    double aError = calcAngle(robotVector, goalPos) - robotVector.a;
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

        if(stopType == StopType::SOFT_STOP){
          leftMotors->move(0);
          rightMotors->move(0);
        } else if(stopType == StopType::HARD_STOP){
          leftMotors->moveVelocity(0);
          rightMotors->moveVelocity(0);
        }
      }
  };
}

Vector Drive::calculateLookaheadPoint(Line line, Vector robotPos){
  double lookDist = 7;
  if(fabs(calcDist(line.b, robotPos)) < lookDist){
    return line.b;
  } else {
    Vector lookPoint;
    double lineSlope = calcSlope(line);
    double normal = calcSlope(calcAngle(line) + (M_PI * 0.5));
    lookPoint.x = (line.a.y - (lineSlope * line.a.x) - robotPos.y + (normal * robotPos.x)) / (normal - lineSlope);
    lookPoint.y = (line.a.y + (lineSlope * lookPoint.x) - (lineSlope * line.a.x));

    double angle = calcAngle(lookPoint, line.b);
    lookPoint.x += lookDist * cos(angle);
    lookPoint.y += lookDist * sin(angle);



    if(lookPoint.x == NAN || lookPoint.y == NAN){
      return line.b;
    }
    return lookPoint;
  }
};



void Drive::Machine::driveToPointLine(double xStart, double yStart, double xGoal, double yGoal, int velocity, StopType stopType, bool reverse, double range){
  this->state = DRIVE_TO_POINT;
  poller = Poller(false);

  Line line;
  line.a.x = xStart;
  line.a.y = yStart;
  line.b.x = xGoal;
  line.b.y = yGoal;
  deccelZone = false;

  this->currentState = [line, velocity, stopType, reverse, range, this]{
    Vector robotVector = *posTracker->getVector();
    Vector lookaheadPoint = calculateLookaheadPoint(line, robotVector);

    double distErrorToGoal = calcDist(robotVector, line.b);
    double aErrorToGoal = calcAngle(robotVector, line.b)  - robotVector.a;
    if(reverse){
      aErrorToGoal -= M_PI * sign(aErrorToGoal); // add 180 degrees if the robot should go backwards
    }
    aErrorToGoal = clipAngle(aErrorToGoal);
    double straightDistToGoal = distErrorToGoal * cos(aErrorToGoal);
    double sidewaysDistToGoal = distErrorToGoal * sin(aErrorToGoal);

    double aErrorToLook = calcAngle(robotVector, lookaheadPoint) - robotVector.a;
    if(reverse){
      aErrorToLook -= M_PI * sign(aErrorToLook); // add 180 degrees if the robot should go backwards
    }
    aErrorToLook = clipAngle(aErrorToLook);

    Polar velocityPolar = vectorToPolar(*posTracker->getVelocity());



    int leftVel, rightVel;


    leftVel = rightVel = velocity;

    if(deccelZone){
      lookaheadPoint = line.b;
      if(stopType == StopType::SOFT_STOP){
        leftVel = rightVel = fabs(limit(straightDistToGoal * kPsl, velocity));
      }
    } else {
      deccelZone = fabs(straightDistToGoal) < fabs((velocityPolar.m * 2));
    }

    int correction = 0;

    int dirrection = 1;
    if(reverse){
      dirrection = -1;
    }

    if(fabs(distErrorToGoal) > noTurnDist && (fabs(straightDistToGoal) > noTurnDist || fabs(sidewaysDistToGoal) > noTurnDist)){
      aIntegral = limit(aIntegral, aIntegralLimit);
      if(fabs(aErrorToLook) > aErrorIntegralRange || sign(aErrorToLook) != sign(aIntegral)){
        aIntegral = 0;
      }
      correction = fabs((aErrorToLook * kPa) + (aIntegral * kIa));
      aIntegral += aErrorToLook;

      switch(sign(aErrorToLook) * dirrection){
        case 1:
          leftVel -= correction;
          break;
        case -1:
          rightVel -= correction;
          break;
        default:
          break;
      }
    } else {
      aIntegral = 0;
    }




    rightVel *= dirrection;
    leftVel *= dirrection;

    int leftVelDelta = limit(leftVel - leftMotors->getTargetVelocity(), 3);
    leftMotors->moveVelocity((leftMotors->getTargetVelocity() + leftVelDelta));

    int rightVelDelta = limit(rightVel - rightMotors->getTargetVelocity(), 3);
    rightMotors->moveVelocity((rightMotors->getTargetVelocity() + rightVelDelta));
    pros::lcd::print(7, "l: %5f, %5d", radToDegree(aErrorToLook), aIntegral * kIa);


    if((fabs(straightDistToGoal) < range || fabs(aErrorToGoal) > (M_PI / 2)) && fabs(distErrorToGoal) < noTurnDist){ // if within 5 inch circle of point and cant get any much closer
      poller.setPoller(true);
      if(stopType == StopType::HARD_STOP){
        leftMotors->moveVelocity(0);
        rightMotors->moveVelocity(0);
      }
    }
  };
};




void Drive::Machine::turnToPoint(double xGoal, double yGoal, int velocity, StopType stopType, double range){
  /*
  this->state = TURN_TO_POINT;
  poller = Poller(false);
  this->currentState = [xGoal, yGoal, velocity, stop, range, this]{
    Vector goalPos;
    goalPos.x = xGoal;
    goalPos.y = yGoal;
    Vector robotVector = *posTracker->getVector();

    double aError = calcAngle(robotVector, goalPos) - robotVector.a;

    int rightVel, leftVel;

    int leftVel, rightVel;
    if(stop){
      leftVel = limit(slError * kPsl, velocity);
      rightVel = limit(slError * kPsl, velocity);
    } else {
      leftVel = velocity;
      rightVel = velocity;
    }

  };*/
};

double Drive::Machine::getDistance(){
  return ((leftMotors->getPosition() + rightMotors->getPosition())/2)*ticksToDistance;
};

double Drive::Machine::getAngle(){
  return (rightMotors->getPosition() - leftMotors->getPosition()) * 0.089;
};

Poller* Drive::Machine::getPoller(){
  return &poller;
};
