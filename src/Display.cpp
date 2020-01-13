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

  pros::lcd::print(0, "%s Loop: %d", Atn::getSelectedAutonName().c_str(), Robot::handleLoopTime);
  pros::lcd::print(1, "liftTilt State: %d", Robot::liftTilt.getState());
  pros::lcd::print(2, "Lift P:%f S:%d V:%3f", Robot::liftMotorGroup.getPosition(), Robot::lift.getState(), Robot::liftMotorGroup.getVoltage());
  pros::lcd::print(3, "Tilt P:%f S:%d V:%3f", Robot::tiltMotorGroup.getPosition(), Robot::tilt.getState(), Robot::tiltMotorGroup.getVoltage());
  pros::lcd::print(4, "Angle: %5f ( %5f )", Robot::posTracker.getAngle(), radToDegree(Robot::posTracker.getAngle()));
  pros::lcd::print(5, "X:%5f Y: %5f", Robot::posTracker.getVector()->x,  Robot::posTracker.getVector()->y);
  pros::lcd::print(6, "clipTest: %5f, %5f", clipAngle(degreeToRad(380)), clipAngle(degreeToRad(-270)));
  //pros::lcd::print(7, "Intake State: %d", Robot::intake.getState());
};
