#include "main.h"

MotorGroup::MotorGroup(std::vector<pros::Motor*> motors, int slew){
  for(pros::Motor* motor:motors){
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

Poller MotorGroup::movePosition(int position, int velocity, int range){
  for(pros::Motor* motor:this->motors){
    motor->move_absolute(position, velocity);
  }
  std::function<double(void)> pos = [this](void){ return this->getPosition(); };
  return Poller(pos, position, range);
};

Poller MotorGroup::moveVelocity(int velocity, int range){
  for(pros::Motor* motor:this->motors){
    motor->move_velocity(velocity);
  }
  std::function<double(void)> vel = [this](void){ return this->getVelocity(); };
  return Poller(vel, velocity, range);
};


void MotorGroup::setZeroPosition(void){
  for(pros::Motor* motor:this->motors){
    motor->tare_position();
  }
};

int MotorGroup::getTargetPosition(void){
  return this->motors.at(0)->get_target_position();
};

int MotorGroup::getPosition(void){
  return this->motors.at(0)->get_position();
};

int MotorGroup::getTargetVelocity(void){
  return this->motors.at(0)->get_target_velocity();
};

int MotorGroup::getVelocity(void){
  return this->motors.at(0)->get_actual_velocity();
};
