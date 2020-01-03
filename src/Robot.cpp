#include "main.h"

const int numMotors = 8;

pros::Motor leftMotor1(1, GEARSET_600_RPM, true);
pros::Motor leftMotor2(19, GEARSET_600_RPM, true);
pros::Motor rightMotor1(2,GEARSET_600_RPM, false);
pros::Motor rightMotor2(3, GEARSET_600_RPM, false);
pros::Motor tiltMotor(9, GEARSET_100_RPM, false);
pros::Motor liftMotor(8, GEARSET_100_RPM, false);
pros::Motor intakeMotor1(6, GEARSET_200_RPM, false);
pros::Motor intakeMotor2(7, GEARSET_200_RPM, true);

std::vector<pros::Motor*> Robot::motors = {&leftMotor1, &leftMotor2, &rightMotor1, &rightMotor2, &tiltMotor, &liftMotor, &intakeMotor1, &intakeMotor2};
std::vector<std::string> Robot::motorNames = {"left1", "left2", "right1", "right2", "tilt", "lift", "intake1", "intake2"};

MotorGroup Robot::intakeMotorGroup({&intakeMotor1, &intakeMotor2});
MotorGroup Robot::liftMotorGroup({&liftMotor}, pros::E_MOTOR_BRAKE_HOLD);
MotorGroup Robot::tiltMotorGroup({&tiltMotor});
MotorGroup Robot::leftMotorGroup({&leftMotor1, &leftMotor2});
MotorGroup Robot::rightMotorGroup({&rightMotor1, &rightMotor2});

Intake::Machine Robot::intake(&intakeMotorGroup);

Lift::Machine Robot::lift(&liftMotorGroup);
Tilt::Machine Robot::tilt(&tiltMotorGroup);
LiftTilt::Machine Robot::liftTilt(&Robot::tilt, &Robot::lift);

Drive::Machine Robot::drive(&leftMotorGroup, &rightMotorGroup);

std::vector<Handleable*> Robot::machines = {&Robot::intake, &Robot::liftTilt, &Robot::lift, &Robot::tilt, &Robot::drive};



void Robot::handle(void){
  drive.handle();
  liftTilt.handle();
  intake.handle();
  Display::update();

  pros::delay(10);
};

void Robot::wait(Poller* poller){
  while(!poller->finished()){
    handle();
  }
};
