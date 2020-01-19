#include "main.h"

using namespace Robot;

int Atn::selectedAuton = 0;
bool Atn::deployed = false;

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
  Robot::drive.reset();
  autons.at(selectedAuton)->run(true);
};

void Atn::deploy(void){
  Poller* liftTiltPoller = liftTilt.getPoller();
  liftTilt.setState(LiftTilt::DEPLOY);
  wait(liftTiltPoller);
  liftTilt.setState(LiftTilt::BOT_INTAKE);

  deployed = true;
}

void Atn::dropStack(void){
  Poller* drivePoller = drive.getPoller();
  Poller* liftTiltPoller = liftTilt.getPoller();
  Poller timer;


  drive.move(0,0);

  liftTilt.setState(LiftTilt::DROP_STACK);
  timer = Poller(500);
  wait(&timer);
  intake.setState(Intake::STOP);

  timer = Poller(4600);
  wait(&timer);
  intake.setState(Intake::STACK_OUTTAKE);
  drive.moveDistance(-7,80);
  wait(drivePoller);
};

Atn::Auton redSmall5(
  [](bool side){
    Poller* drivePoller = drive.getPoller();
    Poller* liftTiltPoller = liftTilt.getPoller();

    intake.setState(Intake::INTAKE);

    Atn::deploy();

    drive.moveDistance(32, 170,Drive::DEF_RANGE,0);

    wait(drivePoller);

    drive.moveDistance(10,150);
    wait(drivePoller);

    drive.moveAngleDeg(30,100); //ANGLE
    wait(drivePoller);

    drive.moveDistance(9,120);
    wait(drivePoller);

    liftTilt.setState(LiftTilt::MID_INTAKE);

    drive.moveDistance(-20,300);
    wait(drivePoller);

    drive.moveAngleDeg(-192,100);//ANGLE
    wait(drivePoller);

    drive.moveDistance(17,300,1,0);
    wait(drivePoller);

    Atn::dropStack();
    Robot::stop();

  },
  "redSmall5"
);


Atn::Auton blueSmall5(
  [](bool side){
    Poller* drivePoller = drive.getPoller();
    Poller* liftTiltPoller = liftTilt.getPoller();

    intake.setState(Intake::INTAKE);
    Atn::deploy();

    drive.moveDistance(32, 170,Drive::DEF_RANGE,0);

    wait(drivePoller);

    drive.moveDistance(10,150);
    wait(drivePoller);

    drive.moveAngleDeg(-30,100); //ANGLE
    wait(drivePoller);

    drive.moveDistance(9,120);
    wait(drivePoller);

    liftTilt.setState(LiftTilt::MID_INTAKE);

    drive.moveDistance(-20,300);
    wait(drivePoller);

    drive.moveAngleDeg(192,100);//ANGLE
    wait(drivePoller);

    drive.moveDistance(17,300,1,0);
    wait(drivePoller);

    Atn::dropStack();
    Robot::stop();
  },
  "blueSmall5"
);


Atn::Auton redLargeZn(
  [](bool side){
    Poller* drivePoller = drive.getPoller();
    Poller* liftTiltPoller = liftTilt.getPoller();

    intake.setState(Intake::INTAKE);
    Atn::deploy();

    drive.moveDistance(21,170,Drive::DEF_RANGE,0);
    intake.setState(Intake::INTAKE);
    wait(drivePoller);

    liftTilt.setState(LiftTilt::PRE_TWO_GRAB);
    drive.moveDistance(11,120);
    wait(drivePoller);

    liftTilt.setState(LiftTilt::BOT_INTAKE);

    drive.moveDistance(10,150);
    wait(drivePoller);

    drive.moveAngleDeg(132,100);//ANGLE
    wait(drivePoller);

    drive.moveDistance(35,300,1,0);
    wait(drivePoller);

    Atn::dropStack();
    Robot::stop();
  },
  "redLargeZn"
);

Atn::Auton blueLargeZn(
  [](bool side){
    Poller* drivePoller = drive.getPoller();
    Poller* liftTiltPoller = liftTilt.getPoller();

    intake.setState(Intake::INTAKE);
    Atn::deploy();

    drive.moveDistance(21,170,Drive::DEF_RANGE,0);
    intake.setState(Intake::INTAKE);
    wait(drivePoller);

    liftTilt.setState(LiftTilt::PRE_TWO_GRAB);
    drive.moveDistance(11,120);
    wait(drivePoller);

    liftTilt.setState(LiftTilt::BOT_INTAKE);

    drive.moveDistance(10,150);
    wait(drivePoller);

    drive.moveAngleDeg(-132,100);//ANGLE
    wait(drivePoller);

    drive.moveDistance(345,300,1,0);
    wait(drivePoller);

    Atn::dropStack();
    Robot::stop();
  },
  "blueLargeZn"
);

Atn::Auton noDrop(
  [](bool side){
    Poller* drivePoller = drive.getPoller();
    Poller* liftTiltPoller = liftTilt.getPoller();

    intake.setState(Intake::INTAKE);
    Atn::deploy();

    drive.moveDistance(30,170,Drive::DEF_RANGE,0);

    wait(drivePoller);

    Robot::stop();
  },
  "smallNoDrop"
);
/*


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


  	drivePoller = drive.moveAngleDeg(137,100);//ANGLE
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


  	drivePoller = drive.moveAngleDeg(-137,100);//ANGLE
  	wait(&drivePoller);

    drivePoller = drive.moveDistance(34.5,300, 1.5,0);
  	wait(&drivePoller);


    Atn::dropStack();
  },
  "blueLarge5"
);
*/
std::vector<Atn::Auton*> Atn::autons = {&redSmall5, &blueSmall5, &redLargeZn, &blueLargeZn, &noDrop};
