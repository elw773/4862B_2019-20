#include "main.h"

typedef int (MotorGroup::*ValueFn)(void);

MotorGroup::MotorGroup(std::vector<pros::Motor*> motors, pros::motor_brake_mode_e brakeMode, int slew){
  for(pros::Motor* motor:motors){
    motor->set_brake_mode(brakeMode);
    this->motors.push_back(motor);
  }
  this->slew = slew;
};

Poller MotorGroup::move(int power){
  for(pros::Motor* motor:this->motors){
    motor->move(power);
  }
  return Poller();
};

Poller MotorGroup::movePosition(double position, int velocity, double range){
  for(pros::Motor* motor:this->motors){
    motor->move_absolute(position, velocity);
  }
  return Poller(std::bind(&MotorGroup::getPosition, this), position, range);
};

double MotorGroup::getVoltage(void){
  return (this->motors.at(0)->get_voltage() / 12000) * 127;
}

Poller MotorGroup::moveVelocity(int velocity, int range){
  for(pros::Motor* motor:this->motors){
    motor->move_velocity(velocity);
  }
  return Poller(std::bind(&MotorGroup::getVelocity, this), velocity, range);
};


void MotorGroup::setZeroPosition(void){
  for(pros::Motor* motor:this->motors){
    motor->tare_position();
  }
};

double MotorGroup::getTargetPosition(void){
  return this->motors.at(0)->get_target_position();
};

double MotorGroup::getPosition(void){
  return this->motors.at(0)->get_position();
};

int MotorGroup::getTargetVelocity(void){
  return this->motors.at(0)->get_target_velocity();
};

int MotorGroup::getVelocity(void){
  return this->motors.at(0)->get_actual_velocity();
};
