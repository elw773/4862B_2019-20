#include "main.h"

int Tilt::stateToPos(State state){

  switch(state){
    case TOWER: return TOWER_POS;
    case BOT_INTAKE: return BOT_INTAKE_POS;
    case MID_INTAKE: return MID_INTAKE_POS;
    case HIGH_INTAKE: return HIGH_INTAKE_POS;
    case DROP_STACK: return DROP_STACK_POS;
    case STOP: return BOT_INTAKE_POS;
    case CALIBRATE: return -1;
  }
};

Tilt::Machine::Machine(MotorGroup* tiltMotors){
  this->tiltMotors = tiltMotors;
};

Poller Tilt::Machine::setState(State state){
  this->state = state;

  switch(state){
    case CALIBRATE: return calibrate(); break;
    case STOP: return tiltMotors->move(0); break;
    default:
      int pos = stateToPos(state);
      this->currentState = [this, pos](void){
        tiltMotors->movePosition(pos, DEF_VELOCITY, DEADBAND);
      };
      return tiltMotors->movePosition(pos, DEF_VELOCITY, DEADBAND);
      break;
  }
};

Poller Tilt::Machine::calibrate(void){
  this->state = CALIBRATE;
  Poller isDone = Poller(false);

  this->currentState = [this, &isDone](void){
    tiltMotors->move(-30);
    if(tiltMotors->getVelocity()){
      tiltMotors->move(0);
      tiltMotors->setZeroPosition();
      isDone.setPoller(true);
    }
  };

  Poller timer = Poller(200);
  return Poller(&timer, &isDone);
};

void Tilt::Machine::handle(void){
  this->currentState();
  if(state != CALIBRATE){
    if(abs(tiltMotors->getPosition() - BOT_INTAKE_POS) < DEADBAND){
      tiltMotors->move(BOT_HOLD_POWER);
    } else if(tiltMotors->getPosition() > SLOW_POS && tiltMotors->getVelocity() > 0){ // if is moving up and it it in slow zone
      tiltMotors->movePosition(tiltMotors->getTargetPosition(), SLOW_VELOCITY, DEADBAND); // go where you were going, but slower now
    }
  }
};
