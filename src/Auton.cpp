#include "main.h"

int Atn::selectedAuton = 0;

Atn::Auton::Auton(std::function<void(bool)> func, std::string name){
  this->func = func;
  this->name = name;
};

void Atn::Auton::run(bool side){
  this->func(side);
};
std::string Atn::Auton::getName(void){
  return name;
};

void Atn::nextAuton(void){
  selectedAuton ++;
  if(selectedAuton > autons.size() - 1){
    selectedAuton = 0;
  }
};
void Atn::prevAuton(void){
  selectedAuton --;
  if(selectedAuton < 0){
    selectedAuton = autons.size() - 1;
  }
};

std::string Atn::getSelectedAutonName(void){
  return autons.at(selectedAuton)->getName();
};

void Atn::runAuton(void){
  autons.at(selectedAuton)->run(true);
};

void Atn::dropStack(void){
  Robot::intake.setState(Intake::STOP);
  Robot::drive.move(0,0);
  Poller liftTiltPoller = Robot::liftTilt.setState(LiftTilt::DROP_STACK);
  liftTiltPoller = Robot::tilt.setState(Tilt::DROP_STACK);
  Robot::wait(&liftTiltPoller);
  liftTiltPoller = Poller(1000);
  Robot::intake.setState(Intake::STACK_OUTTAKE);

  Robot::wait(&liftTiltPoller);
  Poller drivePoller = Robot::drive.moveDistance(-10,60);
  Robot::wait(&drivePoller);
};

Atn::Auton redSmallZn(
  [](bool side){
    Robot::lift.setState(Lift::LOW_TOWER);
    Poller liftPoller = Poller(1000);
    Robot::lift.setState(Lift::INTAKE);
    Poller drivePoller = Robot::drive.moveDistance(33,110);
  	Robot::intake.setState(Intake::INTAKE);
  	Robot::wait(&drivePoller);

  	drivePoller = Robot::drive.moveAngle(20,100);//ANGLE
  	Robot::wait(&drivePoller);

  	drivePoller = Robot::drive.moveDistance(18.5,200);
  	Robot::wait(&drivePoller);
    drivePoller = Poller(200);
    Robot::wait(&drivePoller);

  	drivePoller = Robot::drive.moveDistance(-30,300);
  	Robot::wait(&drivePoller);

  	drivePoller = Robot::drive.moveAngle(-167,100);//ANGLE
  	Robot::wait(&drivePoller);

  	drivePoller = Robot::drive.moveDistance(8.5,200,1,0);
  	Robot::wait(&drivePoller);

    drivePoller = Robot::drive.moveDistance(-1.5,100);
  	Robot::wait(&drivePoller);

    Atn::dropStack();
  },
  "redSmallZn"
);


Atn::Auton blueSmallZn(
  [](bool side){
    Robot::lift.setState(Lift::LOW_TOWER);
    Poller liftPoller = Poller(1000);
    Robot::lift.setState(Lift::INTAKE);
    Poller drivePoller = Robot::drive.moveDistance(33,110);
  	Robot::intake.setState(Intake::INTAKE);
  	Robot::wait(&drivePoller);

  	drivePoller = Robot::drive.moveAngle(-20,100);//ANGLE
  	Robot::wait(&drivePoller);

  	drivePoller = Robot::drive.moveDistance(18.5,200);
  	Robot::wait(&drivePoller);
    drivePoller = Poller(200);
    Robot::wait(&drivePoller);

  	drivePoller = Robot::drive.moveDistance(-30,300);
  	Robot::wait(&drivePoller);

  	drivePoller = Robot::drive.moveAngle(167,100);//ANGLE
  	Robot::wait(&drivePoller);

  	drivePoller = Robot::drive.moveDistance(8.5,200,1,0);
  	Robot::wait(&drivePoller);

    drivePoller = Robot::drive.moveDistance(-1.5,200);
  	Robot::wait(&drivePoller);

    Atn::dropStack();
  },
  "blueSmallZn"
);


Atn::Auton redLargeZn(
  [](bool side){
    Poller drivePoller = Robot::drive.moveDistance(16,170,Drive::DEF_RANGE,0);
    Robot::intake.setState(Intake::INTAKE);
    Robot::wait(&drivePoller);

    Poller liftTiltPoller = Robot::liftTilt.setState(LiftTilt::PRE_TWO_GRAB);
    drivePoller = Robot::drive.moveDistance(16,120);
    Robot::wait(&drivePoller);

    liftTiltPoller = Robot::liftTilt.setState(LiftTilt::BOT_INTAKE);

    drivePoller = Robot::drive.moveDistance(12,150);
    Robot::wait(&drivePoller);

    drivePoller = Robot::drive.moveAngle(30,100); //ANGLE
    Robot::wait(&drivePoller);

    drivePoller = Robot::drive.moveDistance(10,120);
    Robot::wait(&drivePoller);

    drivePoller = Robot::drive.moveAngle(130,100);//ANGLE
    Robot::wait(&drivePoller);

    drivePoller = Robot::drive.moveDistance(40,300,1,0);
    Robot::wait(&drivePoller);

    Atn::dropStack();
  },
  "redLargeZn"
);

Atn::Auton blueLargeZn(
  [](bool side){
    Poller drivePoller = Robot::drive.moveDistance(16,120,Drive::DEF_RANGE,0);
    Robot::intake.setState(Intake::INTAKE);
    Robot::wait(&drivePoller);

    Poller liftTiltPoller = Robot::liftTilt.setState(LiftTilt::PRE_TWO_GRAB);
    drivePoller = Robot::drive.moveDistance(16,120);
    Robot::wait(&drivePoller);

    liftTiltPoller = Robot::liftTilt.setState(LiftTilt::BOT_INTAKE);

    drivePoller = Robot::drive.moveDistance(12,150);
    Robot::wait(&drivePoller);

    drivePoller = Robot::drive.moveAngle(-30,100); //ANGLE
    Robot::wait(&drivePoller);

    drivePoller = Robot::drive.moveDistance(10,120);
    Robot::wait(&drivePoller);

    drivePoller = Robot::drive.moveAngle(-130,100);//ANGLE
    Robot::wait(&drivePoller);

    drivePoller = Robot::drive.moveDistance(40,300,1,0);
    Robot::wait(&drivePoller);


    Atn::dropStack();
  },
  "blueLargeZn"
);


Atn::Auton redLargeNoDrop(
  [](bool side){
    Poller drivePoller = Robot::drive.moveDistance(16,200,Drive::DEF_RANGE,0);
  	Robot::intake.setState(Intake::INTAKE);
  	Robot::wait(&drivePoller);

  	Poller liftTiltPoller = Robot::liftTilt.setState(LiftTilt::PRE_TWO_GRAB);
  	drivePoller = Robot::drive.moveDistance(16,200);
  	Robot::wait(&drivePoller);

  	liftTiltPoller = Robot::liftTilt.setState(LiftTilt::BOT_INTAKE);


    	drivePoller = Robot::drive.moveDistance(6,150);
    	Robot::wait(&drivePoller);


  	drivePoller = Robot::drive.moveAngle(137,100);//ANGLE
  	Robot::wait(&drivePoller);

  	drivePoller = Robot::drive.moveDistance(34.5,300, 1.5,0);
  	Robot::wait(&drivePoller);

    Atn::dropStack();
  },
  "redLarge5"
);

Atn::Auton blueLargeNoDrop(
  [](bool side){
    Poller drivePoller = Robot::drive.moveDistance(16,200,Drive::DEF_RANGE,0);
  	Robot::intake.setState(Intake::INTAKE);
  	Robot::wait(&drivePoller);

  	Poller liftTiltPoller = Robot::liftTilt.setState(LiftTilt::PRE_TWO_GRAB);
  	drivePoller = Robot::drive.moveDistance(16,200);
  	Robot::wait(&drivePoller);

  	liftTiltPoller = Robot::liftTilt.setState(LiftTilt::BOT_INTAKE);


    	drivePoller = Robot::drive.moveDistance(6,150);
    	Robot::wait(&drivePoller);


  	drivePoller = Robot::drive.moveAngle(-137,100);//ANGLE
  	Robot::wait(&drivePoller);

    drivePoller = Robot::drive.moveDistance(34.5,300, 1.5,0);
  	Robot::wait(&drivePoller);


    Atn::dropStack();
  },
  "blueLarge5"
);

std::vector<Atn::Auton*> Atn::autons = {&redSmallZn, &blueSmallZn, &redLargeZn, &blueLargeZn, &redLargeNoDrop, &blueLargeNoDrop};
