#include "main.h"

Display::DisplayBtn Display::upBtn(std::bind(pros::c::adi_digital_read,8));
Display::DisplayBtn Display::dnBtn(std::bind(pros::c::adi_digital_read,7));

Display::DisplayBtn::DisplayBtn(std::function<bool(void)> isPressed){
  this->isPressed = isPressed;
};

bool Display::DisplayBtn::isClicked(void){
  bool out = !wasPressed && isPressed();
  wasPressed = isPressed();
  return out;
};

void Display::init(void){
  pros::lcd::initialize();
};

void Display::update(void){
  if(upBtn.isClicked()){
    Atn::nextAuton();
  }
  if(dnBtn.isClicked()){
    Atn::prevAuton();
  }

  pros::lcd::print(0, "Auton: %s", Atn::getSelectedAutonName().c_str());
  pros::lcd::print(1, "Angle: %f, Distance: %f", Robot::drive.getAngle(), Robot::drive.getDistance());
  pros::lcd::print(2, "liftTilt State: %d", Robot::liftTilt.getState());
  pros::lcd::print(3, "Lift Pos:%f State: %d", Robot::liftMotorGroup.getPosition(), Robot::lift.getState());
  pros::lcd::print(4, "Tilt Pos:%f State: %d", Robot::tiltMotorGroup.getPosition(), Robot::tilt.getState());
  pros::lcd::print(5, "Intake State: %d", Robot::intake.getState());
};
