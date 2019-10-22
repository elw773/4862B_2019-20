#include "main.h"

Tilt::Machine::Machine(MotorGroup* tiltMotors){
  this->tiltMotors = tiltMotors;
};

Poller Tilt::Machine::setState(State state){

};

Poller Tilt::Machine::dropStack(void){
  std::function<double(void)> pos = [this](void){ return tiltMotors->getPosition(); };
  Poller poller = Poller(pos, DROP_STACK, DEADBAND);
  this->currentState = [this](void){
    tiltMotors->movePosition(DROP_STACK);
  };
  return poller;
};

Poller Tilt::Machine::bottom(void){
  std::function<double(void)> pos = [this](void){ return tiltMotors->getPosition(); };
  Poller poller = Poller(pos, BOT_INTAKE, DEADBAND);
  this->currentState = [this, poller](void){
    tiltMotors->movePosition(BOT_INTAKE);
    if(poller.finished()){
      tiltMotors->move(BOT_POWER);
    }
  };
  return poller;
};

Poller Tilt::Machine::calibrate(void){
  this->currentState = [this](void){
    tiltMotors->move(-30);
  }
  Poller isDone = Poller([this](int*){
    return tiltMotors->getVelocity() == 0;
  });
  return Poller(&Poller(200), &isDone);
};
