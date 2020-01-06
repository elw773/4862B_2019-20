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
  pros::lcd::print(1, "Angle: %5f ( %5f )", Robot::posTracker.getAngle(), Robot::posTracker.getAngle() / 0.0174532925);
  pros::lcd::print(2, "Str:%5f Sw: %5f", Robot::posTracker.getStraightDist(),  Robot::posTracker.getSidewaysDist());
  pros::lcd::print(3, "X:%5f Y: %5f", Robot::posTracker.getVector()->x,  Robot::posTracker.getVector()->y);
  pros::lcd::print(4, "liftTilt State: %d", Robot::liftTilt.getState());
  pros::lcd::print(5, "Lift Pos:%f State: %d", Robot::liftMotorGroup.getPosition(), Robot::lift.getState());
  pros::lcd::print(6, "Tilt Pos:%f State: %d", Robot::tiltMotorGroup.getPosition(), Robot::tilt.getState());
  //pros::lcd::print(7, "Intake State: %d", Robot::intake.getState());
};
