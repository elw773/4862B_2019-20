#include "main.h"

MotorGroup::MotorGroup(std::vector<pros::Motor*> motors, int slew){
  for(pros::Motor* motor:motors){
    this->motors.push_back(motor);
    motor->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
  }
  this->slew = slew;
};

void MotorGroup::move(int power){
  for(pros::Motor* motor:this->motors){
    motor->move(power);
  }
};

void MotorGroup::movePosition(double position, int velocity){
  for(pros::Motor* motor:this->motors){
    motor->move_absolute(position, velocity);
  }
};

void MotorGroup::moveVelocity(int velocity){
  for(pros::Motor* motor:this->motors){
    motor->move_velocity(velocity);
  }
};


void MotorGroup::setZeroPosition(void){
  for(pros::Motor* motor:this->motors){
    motor->tare_position();
  }
};

double MotorGroup::getTargetPosition(void){
  return this->motors.at(0)->get_target_position();
};

int MotorGroup::getTargetVelocity(void){
  return this->motors.at(0)->get_target_velocity();
};

int MotorGroup::getVelocity(void){
  return this->motors.at(0)->get_actual_velocity();
};
