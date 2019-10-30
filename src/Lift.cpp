#include "main.h"

int Lift::holdPower = Lift::INTAKE_HOLD_POWER;

int Lift::stateToPos(State state){
  switch(state){
    case ALLIANCE_TOWER: return ALLIANCE_TOWER_POS;
    case LOW_TOWER: return LOW_TOWER_POS;
    case MID_TOWER: return MID_TOWER_POS;
    case CALIBRATE: return -1;
    default: return INTAKE_POS;
  }
};

Lift::Machine::Machine(MotorGroup* liftMotors){
  this->liftMotors = liftMotors;
};

Poller Lift::Machine::setState(State state){
  this->state = state;
  switch(state){
    case CALIBRATE: return calibrate(); break;
    case STOP: return liftMotors->move(0); break;
    case INTAKE: holdPower = INTAKE_HOLD_POWER; break;
    case DROP_STACK: holdPower = DROP_STACK_HOLD_POWER; break;
  }
  int pos = stateToPos(state);
  this->currentState = [this, pos](void){
    liftMotors->movePosition(pos, DEF_VELOCITY, DEADBAND);
  };
  return liftMotors->movePosition(pos, DEF_VELOCITY, DEADBAND);
};


Poller Lift::Machine::calibrate(void){
  this->state = CALIBRATE;
  Poller isDone = Poller(false);

  this->currentState = [this, &isDone](void){
    liftMotors->move(-30);
    if(liftMotors->getVelocity()){
      liftMotors->move(0);
      liftMotors->setZeroPosition();
      isDone.setPoller(true);
    }
  };

  Poller timer = Poller(200);
  return Poller(&timer, &isDone);
};


void Lift::Machine::handle(void){
  this->currentState();
  if(state != CALIBRATE){
    if(abs(liftMotors->getPosition() - INTAKE_POS) < DEADBAND){
      liftMotors->move(holdPower);
    }
  }
};
