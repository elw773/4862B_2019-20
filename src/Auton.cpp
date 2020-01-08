#include "main.h"

using namespace Robot;

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

void dropStack(void){
  Poller* drivePoller = drive.getPoller();
  Poller* liftTiltPoller = liftTilt.getPoller();
  Poller timer;

  intake.setState(Intake::STOP);
  drive.move(0,0);
  liftTilt.setState(LiftTilt::DROP_STACK);
  wait(liftTiltPoller);
  timer = Poller(700);
  intake.setState(Intake::STACK_OUTTAKE);
  wait(&timer);
  drive.moveDistance(-7,90);
  wait(drivePoller);
};

Atn::Auton redSmallZn(
  [](bool side){Poller* drivePoller = drive.getPoller();
    intake.setState(Intake::INTAKE);
    drive.driveToPoint(40, 0, 170, Drive::StopType::SOFT_STOP, false);
    wait(drivePoller);
    drive.turnToPoint(63, -10, 200, Drive::StopType::SOFT_STOP);
    wait(drivePoller);

    drive.moveDistance(7, 200);
    wait(drivePoller);

    drive.driveToPoint(20, 0, 220, Drive::StopType::SOFT_STOP, true);
    wait(drivePoller);

    drive.turnToAngle(degreeToRad(130)	, 200, Drive::StopType::SOFT_STOP);
    wait(drivePoller);

    drive.moveDistance(10, 150);
    wait(drivePoller);

    intake.setState(Intake::STOP);

    Atn::dropStack();

  },
  "redSmallZn"
);


Atn::Auton blueSmallZn(
  [](bool side){
  intake.setState(Intake::INTAKE);
  drive.driveToPoint(40, 0, 170, Drive::StopType::SOFT_STOP, false);
  wait(drivePoller);
  drive.turnToPoint(63, 10, 200, Drive::StopType::SOFT_STOP);
  wait(drivePoller);

  drive.moveDistance(7, 200);
  wait(drivePoller);

  drive.driveToPoint(20, 0, 220, Drive::StopType::SOFT_STOP, true);
  wait(drivePoller);

  drive.turnToAngle(degreeToRad(-130)	, 200, Drive::StopType::SOFT_STOP);
  wait(drivePoller);

  drive.moveDistance(10, 150);
  wait(drivePoller);

  intake.setState(Intake::STOP);

  Atn::dropStack();
  },
  "blueSmallZn"
);
/*

Atn::Auton redLargeZn(
  [](bool side){
    Poller drivePoller = drive.moveDistance(16,170,Drive::DEF_RANGE,0);
    intake.setState(Intake::INTAKE);
    wait(&drivePoller);

    Poller liftTilt.setState(LiftTilt::PRE_TWO_GRAB);
    drivePoller = drive.moveDistance(16,120);
    wait(&drivePoller);

    liftTilt.setState(LiftTilt::BOT_INTAKE);

    drivePoller = drive.moveDistance(12,150);
    wait(&drivePoller);

    drivePoller = drive.moveAngle(30,100); //ANGLE
    wait(&drivePoller);

    drivePoller = drive.moveDistance(10,120);
    wait(&drivePoller);

    drivePoller = drive.moveAngle(130,100);//ANGLE
    wait(&drivePoller);

    drivePoller = drive.moveDistance(40,300,1,0);
    wait(&drivePoller);

    Atn::dropStack();
  },
  "redLargeZn"
);

Atn::Auton blueLargeZn(
  [](bool side){
    Poller drivePoller = drive.moveDistance(16,120,Drive::DEF_RANGE,0);
    intake.setState(Intake::INTAKE);
    wait(&drivePoller);

    Poller liftTilt.setState(LiftTilt::PRE_TWO_GRAB);
    drivePoller = drive.moveDistance(16,120);
    wait(&drivePoller);

    liftTilt.setState(LiftTilt::BOT_INTAKE);

    drivePoller = drive.moveDistance(12,150);
    wait(&drivePoller);

    drivePoller = drive.moveAngle(-30,100); //ANGLE
    wait(&drivePoller);

    drivePoller = drive.moveDistance(10,120);
    wait(&drivePoller);

    drivePoller = drive.moveAngle(-130,100);//ANGLE
    wait(&drivePoller);

    drivePoller = drive.moveDistance(40,300,1,0);
    wait(&drivePoller);


    Atn::dropStack();
  },
  "blueLargeZn"
);


Atn::Auton redLargeNoDrop(
  [](bool side){
    Poller drivePoller = drive.moveDistance(16,200,Drive::DEF_RANGE,0);
  	intake.setState(Intake::INTAKE);
  	wait(&drivePoller);

  	Poller liftTilt.setState(LiftTilt::PRE_TWO_GRAB);
  	drivePoller = drive.moveDistance(16,200);
  	wait(&drivePoller);

  	liftTilt.setState(LiftTilt::BOT_INTAKE);


    	drivePoller = drive.moveDistance(6,150);
    	wait(&drivePoller);


  	drivePoller = drive.moveAngle(137,100);//ANGLE
  	wait(&drivePoller);

  	drivePoller = drive.moveDistance(34.5,300, 1.5,0);
  	wait(&drivePoller);

    Atn::dropStack();
  },
  "redLarge5"
);

Atn::Auton blueLargeNoDrop(
  [](bool side){
    Poller drivePoller = drive.moveDistance(16,200,Drive::DEF_RANGE,0);
  	intake.setState(Intake::INTAKE);
  	wait(&drivePoller);

  	Poller liftTilt.setState(LiftTilt::PRE_TWO_GRAB);
  	drivePoller = drive.moveDistance(16,200);
  	wait(&drivePoller);

  	liftTilt.setState(LiftTilt::BOT_INTAKE);


    	drivePoller = drive.moveDistance(6,150);
    	wait(&drivePoller);


  	drivePoller = drive.moveAngle(-137,100);//ANGLE
  	wait(&drivePoller);

    drivePoller = drive.moveDistance(34.5,300, 1.5,0);
  	wait(&drivePoller);


    Atn::dropStack();
  },
  "blueLarge5"
);
*/
std::vector<Atn::Auton*> Atn::autons = {&redSmallZn};
