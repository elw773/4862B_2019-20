#include "main.h"

Tilt::Machine::Machine(MotorGroup* tiltMotors){
  this->tiltMotors = tiltMotors;
};

Poller Tilt::Machine::setState(State state){
  switch(state){
    case CALIBRATE: return calibrate(); break;
    case DROP_STACK: return dropStack(); break;
    case BOT_INTAKE: return bottom(); break;
    case STOP: return tiltMotors->move(0); break;
    default:
      this->currentState = [this, state](void){
        tiltMotors->movePosition(state, DEF_VELOCITY, DEADBAND);
      };
      return tiltMotors->movePosition(state, DEF_VELOCITY, DEADBAND);
      break;
  }
};

Poller Tilt::Machine::dropStack(void){
  Poller poller = tiltMotors->movePosition(DROP_STACK, DEF_VELOCITY, DEADBAND);
  this->currentState = [this](void){
    tiltMotors->movePosition(DROP_STACK, DEF_VELOCITY, DEADBAND);
    if(tiltMotors->getPosition() > SLOW_SPOT){
      tiltMotors->movePosition(DROP_STACK, SLOW_VELOCITY, DEADBAND);
    }
  };
  return poller;
};

Poller Tilt::Machine::bottom(void){
  Poller poller = tiltMotors->movePosition(BOT_INTAKE, DEF_VELOCITY, DEADBAND);
  this->currentState = [this, &poller](void){
    tiltMotors->movePosition(BOT_INTAKE, DEF_VELOCITY, DEADBAND);
    if(poller.finished()){
      tiltMotors->move(BOT_POWER);
    }
  };
  return poller;
};

Poller Tilt::Machine::calibrate(void){
  this->currentState = [this](void){
    tiltMotors->move(-30);
  };
  Poller isDone = Poller([this](int*){
    return tiltMotors->getVelocity() == 0;
  });
  Poller timer = Poller(200);
  return Poller(&timer, &isDone);
};
