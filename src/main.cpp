#include "main.h"

int TILT_DOWN = 200;
int TILT_MID = 1000;
int TILT_UP = 4400;

int LIFT_DOWN = 100;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
 void opcontrol() {
	 /*
 	pros::Controller master(pros::E_CONTROLLER_MASTER);
   pros::lcd::initialize();

   int left, right;
   pros::Motor left1(1, MOTOR_GEARSET_06, true);
   pros::Motor left2(2, MOTOR_GEARSET_06, false);
   pros::Motor right1(3,MOTOR_GEARSET_06, false);
   pros::Motor right2(4,MOTOR_GEARSET_06, true);
   pros::Motor tilter(9, MOTOR_GEARSET_36, false);
   pros::Motor lift(5, MOTOR_GEARSET_36, false);
   pros::Motor intake1(8, MOTOR_GEARSET_18, false);
   pros::Motor intake2(7, MOTOR_GEARSET_18, true);

  bool tilt_pressed = false;
  int tilt_goal = 0;

 	while (true) {
     pros::lcd::print(0, "Lift: %f", lift.get_position());
     pros::lcd::print(1, "Tilt: %f", tilter.get_position());


     left = master.get_analog(ANALOG_RIGHT_Y) + master.get_analog(ANALOG_RIGHT_X);
     right = master.get_analog(ANALOG_RIGHT_Y) - master.get_analog(ANALOG_RIGHT_X);

     right1.move(right);
     right2.move(right);
     left1.move(left);
     left2.move(left);

 		if(master.get_digital(DIGITAL_R1)){
 			if(master.get_digital(DIGITAL_R2)){
 				intake1.move(40);
 				intake2.move(40);
 			}else{
 				intake1.move(127);
 				intake2.move(127);
 			}
 		} else if(master.get_digital(DIGITAL_R2)){
 			intake1.move(-70);
 			intake2.move(-70);
 		} else {
 			intake1.move(0);
 			intake2.move(0);
 		}

    if (lift.get_position() < LIFT_DOWN && master.get_analog(ANALOG_LEFT_Y) < 2) {
      lift.move(-4);
    } else {
      lift.set_brake_mode(MOTOR_BRAKE_HOLD);
      lift.move(master.get_analog(ANALOG_LEFT_Y));
    }

    if(!(master.get_digital(DIGITAL_L1) || master.get_digital(DIGITAL_L2))){
      tilt_pressed = false;
    }

    int delta = 0;
    if(!tilt_pressed){
      if(master.get_digital(DIGITAL_L1)){
        tilt_pressed = true;
        delta = 1;
      } else if(master.get_digital(DIGITAL_L2)){
        tilt_pressed = true;
        delta = -1;
      }
    }
    tilt_goal += delta;

    switch(tilt_goal){
      case -1:
        tilt_goal = 0;
        break;
      case 0:
        tilter.move_absolute(0, 60);
        if(tilter.get_position() < TILT_DOWN){
          tilter.move(-1);
        }
        break;
      case 1:
        tilter.move_absolute(TILT_MID, 60);
        break;
      case 2:
        tilter.move_absolute(TILT_UP, 35);
        lift.move(-15);
        break;
      case 3:
        tilt_goal = 2;
        break;
    }

 		pros::delay(20);
 	}*/

	//Poller calibrate = Robot::liftTilt.setState(LiftTilt::CALIBRATE);
	//Robot::wait(&calibrate);
	Robot::liftTilt.setState(LiftTilt::BOT_INTAKE);

	while(true){
		pros::lcd::print(0, "Lift: %d", Robot::liftMotorGroup.getPosition());
		pros::lcd::print(1, "Tilt: %d", Robot::tiltMotorGroup.getPosition());

		Robot::liftTilt.setState(Input::getLiftTiltState());

		Robot::intake.setState(Input::getIntakeState());

		Robot::drive.move(Input::getleftDrive(), Input::getRightDrive());
		Robot::handle();
		pros::delay(20);
	}
 }
