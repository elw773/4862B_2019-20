#include "main.h"

const int numMotors = 8;

long Robot::lastHandleTime = 0;
long Robot::handleLoopTime = 0;

pros::Motor leftMotor1(1, GEARSET_600_RPM, true);
pros::Motor leftMotor2(19, GEARSET_600_RPM, true);
pros::Motor rightMotor1(2,GEARSET_600_RPM, false);
pros::Motor rightMotor2(3, GEARSET_600_RPM, false);
pros::Motor tiltMotor(9, GEARSET_100_RPM, true);
pros::Motor liftMotor(8, GEARSET_100_RPM, false);
pros::Motor intakeMotor1(13, GEARSET_200_RPM, false);
pros::Motor intakeMotor2(17, GEARSET_200_RPM, true);

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



int straightEncPort1 = 1;
int straightEncPort2 = 2;
bool straightReversed = false;
int sidewaysEncPort1 = 5;
int sidewaysEncPort2 = 6;
bool sidewaysReversed = false;
int imuPort = 20;
double straightWheelOffset = -1.5;
double sidewaysWheelOffset = 10;
double straightTicksToInches = 100.0/4085.0;
double sidewaysTicsToInches = 100.0/4085.0;

PosTrack::PosTracker Robot::posTracker(straightEncPort1, straightEncPort2, straightReversed,
            sidewaysEncPort1, sidewaysEncPort2, sidewaysReversed,
            imuPort,
            straightWheelOffset,
            sidewaysWheelOffset,
            straightTicksToInches,
            sidewaysTicsToInches);

Drive::Machine Robot::drive(&leftMotorGroup, &rightMotorGroup, &posTracker);

std::vector<Handleable*> Robot::machines = {&Robot::intake, &Robot::liftTilt, &Robot::lift, &Robot::tilt, &Robot::drive};

void Robot::stop(void){
  for(int i = 0; i < 8; i++){
    motors[i]->move(0);
  }
};

void Robot::handle(void){
  handleLoopTime = pros::millis() - lastHandleTime;
  lastHandleTime = pros::millis();

  Display::update(); // update display first so other things can rewrite


  drive.handle();
  liftTilt.handle();
  intake.handle();
  posTracker.update();
  if(!pros::competition::is_autonomous()){
    if(Input::controller.get_digital(DIGITAL_X) && Input::controller.get_digital(DIGITAL_A)){
      stop();
    }
  }

  pros::delay(10);
};

void Robot::wait(Poller* poller){
  while(!poller->finished()){
    handle();
  }
};
