#include "main.h"

const double aIntegralLimit = 40;
const double aErrorIntegralRange = degreeToRad(30);
const double kIa = 3;
const double kPa = 300;
const double kPsl = 25;
const double kPline = 17;
const double noTurnDist = 7;
const double decelK = 2;
const double decelKa = degreeToRad(10);

void Drive::Machine::reset(void){
  leftMotors->setZeroPosition();
  rightMotors->setZeroPosition();
  posTracker->reset();
}

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
  int leftGoal = tickGoal + leftMotors->getPosition();
  int rightGoal = tickGoal + rightMotors->getPosition();
  poller = Poller(false);

  this->currentState = [leftGoal, rightGoal, velocity, range, this](void){
    /*
    slewVelocity(
      limit(kPsl * (leftGoal - leftMotors->getPosition()) * ticksToDistance, velocity),
      limit(kPsl * (rightGoal - rightMotors->getPosition()) * ticksToDistance, velocity)
    );*/
    leftMotors->movePosition(leftGoal, velocity);
    rightMotors->movePosition(rightGoal, velocity);
    if(fabs(leftGoal - leftMotors->getPosition()) < range * distanceToTicks){
      poller.setPoller(true);
      slewVelocity(0,0);
    }
  };

};

void Drive::Machine::moveAngleDeg(double angle, int velocity, double range, int timeout){
  this->state = ANGLE;
  double tickGoal = angle*angleToTicks;
  int leftGoal = (tickGoal * -0.5) + leftMotors->getPosition();
  int rightGoal = (tickGoal * 0.5) + rightMotors->getPosition();
  poller = Poller(false);

  this->currentState = [leftGoal, rightGoal, velocity, range, this](void){
    leftMotors->movePosition(leftGoal, velocity);
    rightMotors->movePosition(rightGoal, velocity);
    if(fabs(leftGoal - leftMotors->getPosition()) < range * distanceToTicks){
      poller.setPoller(true);
    }
  };
};


void Drive::Machine::slewVelocity(int left, int right){
  int leftVelDelta = limit(left - leftMotors->getTargetVelocity(), 3);
  leftMotors->moveVelocity(leftMotors->getTargetVelocity() + leftVelDelta);

  int rightVelDelta = limit(right - rightMotors->getTargetVelocity(), 3);
  rightMotors->moveVelocity(rightMotors->getTargetVelocity() + rightVelDelta);
};

void Drive::Machine::driveToPointLine(double xGoal, double yGoal, int velocity, StopType stopType, bool reverse, double range){
  driveToPointLine(posTracker->getVector()->x, posTracker->getVector()->y, xGoal, yGoal, velocity, stopType, reverse);
}

Vector Drive::calculateLookaheadPoint(Line line, Vector robotPos){
  double lookDist = 9;
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
    leftVel = rightVel;

    leftVel = rightVel = fabs(limit(straightDistToGoal * kPsl, velocity));

    if(fabs(aErrorToLook) < degreeToRad(10) ){
      if(stopType == StopType::DRIVE_THROUGH){
        leftVel = rightVel = velocity;
      }
    }

    if(deccelZone){
      lookaheadPoint = line.b;
    } else {
      deccelZone = fabs(straightDistToGoal) < fabs((velocityPolar.m * decelK));
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

    slewVelocity(leftVel, rightVel);

    pros::lcd::print(6, "Goal X: %3f Y: %3f", line.b.x, line.b.y);
    pros::lcd::print(7, "StrE: %5f AbsE: %5f", straightDistToGoal, distErrorToGoal);

    if((fabs(straightDistToGoal) < range || fabs(aErrorToGoal) > (M_PI / 2)) && fabs(distErrorToGoal) < noTurnDist){ // if within 5 inch circle of point and cant get any much closer
      poller.setPoller(true);
      if(stopType == StopType::HARD_STOP){
        leftMotors->moveVelocity(0);
        rightMotors->moveVelocity(0);
      }
    }
  };
};


void Drive::Machine::driveToPoint(double xGoal, double yGoal, int velocity, StopType stopType, bool reverse, double range){
  this->state = DRIVE_TO_POINT;
  poller = Poller(false);
  deccelZone = false;

  Vector goal;
  goal.x = xGoal;
  goal.y = yGoal;
  deccelZone = false;

  this->currentState = [goal, velocity, stopType, reverse, range, this]{
    Vector robotVector = *posTracker->getVector();

    double distErrorToGoal = calcDist(robotVector, goal);
    double aErrorToGoal = calcAngle(robotVector, goal)  - robotVector.a;
    if(reverse){
      aErrorToGoal -= M_PI * sign(aErrorToGoal); // add 180 degrees if the robot should go backwards
    }
    aErrorToGoal = clipAngle(aErrorToGoal);
    double straightDistToGoal = distErrorToGoal * cos(aErrorToGoal);
    double sidewaysDistToGoal = distErrorToGoal * sin(aErrorToGoal);

    Polar velocityPolar = vectorToPolar(*posTracker->getVelocity());



    int leftVel, rightVel;
    leftVel = rightVel = velocity;

    if(deccelZone){
      if(stopType == StopType::SOFT_STOP){
        leftVel = rightVel = fabs(limit(straightDistToGoal * kPsl, velocity));
      }
    } else {
      deccelZone = fabs(straightDistToGoal) < fabs((velocityPolar.m * decelK));
    }

    int correction = 0;

    int dirrection = 1;
    if(reverse){
      dirrection = -1;
    }

    if(fabs(distErrorToGoal) > noTurnDist && (fabs(straightDistToGoal) > noTurnDist || fabs(sidewaysDistToGoal) > noTurnDist)){
      aIntegral = limit(aIntegral, aIntegralLimit);
      if(fabs(aErrorToGoal) > aErrorIntegralRange || sign(aErrorToGoal) != sign(aIntegral)){
        aIntegral = 0;
      }
      correction = fabs((aErrorToGoal * kPa) + (aIntegral * kIa));
      aIntegral += aErrorToGoal;

      switch(sign(aErrorToGoal) * dirrection){
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

    pros::lcd::print(6, "Goal X: %3f Y: %3f", goal.x, goal.y);
    pros::lcd::print(7, "StrE: %5f AbsE: %5f", straightDistToGoal, distErrorToGoal);


    rightVel *= dirrection;
    leftVel *= dirrection;

    slewVelocity(leftVel, rightVel);


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
  this->state = TURN_TO_POINT;
  poller = Poller(false);
  deccelZone = false;

  this->currentState = [xGoal, yGoal, velocity, stopType, range, this]{
    Vector goalPos;
    goalPos.x = xGoal;
    goalPos.y = yGoal;
    Vector robotVector = *posTracker->getVector();

    double aError = clipAngle(calcAngle(robotVector, goalPos) - robotVector.a);

    int rightVel, leftVel;

    int pow;

    aIntegral = limit(aIntegral, aIntegralLimit);
    if(fabs(aError) > aErrorIntegralRange || sign(aError) != sign(aIntegral)){
      aIntegral = 0;
    }

    pow = limit((aError * kPa) + (aIntegral * kIa), velocity);
    aIntegral += aError;


    leftVel = -0.5 * pow;
    rightVel = 0.5 * pow;

    slewVelocity(leftVel, rightVel);

    if(fabs(aError) < range){
      poller.setPoller(true);
      if(stopType == StopType::HARD_STOP){
        leftMotors->moveVelocity(0);
        rightMotors->moveVelocity(0);
      }
    }
    pros::lcd::print(6, "Goal X:%3f Y:%3f A:%5f", goalPos.x, goalPos.y, calcAngle(robotVector, goalPos));
    pros::lcd::print(7, "AErr: %5f", aError);


  };
};

void Drive::Machine::turnToAngle(double aGoal, int velocity, StopType stopType, double range){
  this->state = TURN_TO_POINT;
  poller = Poller(false);
  deccelZone = false;
  this->currentState = [aGoal, velocity, stopType, range, this]{
    Vector robotVector = *posTracker->getVector();

    double aError = aGoal - robotVector.a;

    int rightVel, leftVel;

    int pow;

    aIntegral = limit(aIntegral, aIntegralLimit);
    if(fabs(aError) > aErrorIntegralRange || sign(aError) != sign(aIntegral)){
      aIntegral = 0;
    }

    pow = limit((aError * kPa) + (aIntegral * kIa), velocity);
    aIntegral += aError;


    leftVel = -0.5 * pow;
    rightVel = 0.5 * pow;

    slewVelocity(leftVel, rightVel);
    pros::lcd::print(6, "Goal A: ", aGoal);
    pros::lcd::print(7, "AErr: %5f", aError);


    if(fabs(aError) < range){
      poller.setPoller(true);
      if(stopType == StopType::HARD_STOP){
        leftMotors->moveVelocity(0);
        rightMotors->moveVelocity(0);
      }
    }

  };
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
