#include "main.h"

double Tilt::Machine::stateToPos(State state){

  switch(state){
    case TOWER: return TOWER_POS;
    case BOT_INTAKE: return BOT_INTAKE_POS;
    case MID_INTAKE: return MID_INTAKE_POS;
    case HIGH_INTAKE: return HIGH_INTAKE_POS;
    case DROP_STACK: return DROP_STACK_POS;
    case STOP: return BOT_INTAKE_POS;
    case CALIBRATE: return -1;
    case LIFT_POWER: return tiltMotors->getTargetPosition();
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
    case DROP_STACK: return dropStack(); break;
    case LIFT_POWER: return Poller(); break;
    default:
      double pos = stateToPos(state);
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
    tiltMotors->move(-5);
    if(tiltMotors->getVelocity()){
      tiltMotors->move(0);
      tiltMotors->setZeroPosition();
      isDone.setPoller(true);
    }
  };

  Poller timer = Poller(200);
  return Poller(&timer, &isDone);
};

Poller Tilt::Machine::dropStack(void){
  this->state = DROP_STACK;
  this->currentState = [this](void){
      tiltMotors->movePosition(DROP_STACK_POS, SLOW_VELOCITY, DEADBAND);
  };

  return tiltMotors->movePosition(DROP_STACK_POS, SLOW_VELOCITY, DEADBAND+80, 1000);
};

void Tilt::Machine::handle(void){
  if(tiltMotors->getPosition() < BOT_INTAKE_POS + DEADBAND && state == BOT_INTAKE){
    tiltMotors->move(BOT_HOLD_POWER);
  }/* else if(tiltMotors->getPosition() > SLOW_POS && state == DROP_STACK){ // if is moving up and it it in slow zone
    tiltMotors->movePosition(DROP_STACK_POS, SLOW_VELOCITY, DEADBAND); // go where you were going, but slower now
  }*/ else {
    this->currentState();

  }
};
