#include "main.h"

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
    case INTAKE: return moveAndHold(INTAKE, INTAKE_HOLD_POWER); break;
    case DROP_STACK: return moveAndHold(DROP_STACK, DROP_STACK_HOLD_POWER); break;
    case STOP: return liftMotors->move(0); break;
    default:
      int pos = stateToPos(state);
      this->currentState = [this, pos](void){
        liftMotors->movePosition(pos, DEF_VELOCITY, DEADBAND);
      };
      return liftMotors->movePosition(pos, DEF_VELOCITY, DEADBAND);
      break;
  }
};

Poller Lift::Machine::moveAndHold(double position, int holdPower){
  Poller poller = liftMotors->movePosition(position, DEF_VELOCITY, DEADBAND);
  this->currentState = [this, position, holdPower, &poller](void){
    liftMotors->movePosition(position, DEF_VELOCITY, DEADBAND);
    if(poller.finished()){
      liftMotors->move(holdPower);
    }
  };
  return poller;
};

Poller Lift::Machine::calibrate(void){
  this->state = CALIBRATE;
  this->currentState = [this](void){
    liftMotors->move(-30);
    if(liftMotors->getVelocity() == 0){
      liftMotors->setZeroPosition();
    }
  };
  Poller isDone = Poller([this](int*){
    return liftMotors->getVelocity() == 0;
  });
  Poller timer = Poller(200);
  return Poller(&timer, &isDone);
};
