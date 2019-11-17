#include "main.h"

int Lift::holdPower = Lift::INTAKE_HOLD_POWER;

double Lift::Machine::stateToPos(State state){
  switch(state){
    case PRE_TWO_GRAB: return PRE_TWO_GRAB_POS;
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
    case GRAB_STACK: return grabStack(); break;
    case LIFT_POWER: return movePower(Input::getLiftPower()); break;
    case INTAKE: holdPower = INTAKE_HOLD_POWER; break;
    case DROP_STACK: holdPower = DROP_STACK_HOLD_POWER; break;
  }
  double pos = stateToPos(state);
  this->currentState = [this, pos](void){
    liftMotors->movePosition(pos, DEF_VELOCITY, DEADBAND);
  };
  return liftMotors->movePosition(pos, DEF_VELOCITY, DEADBAND);
};


Poller Lift::Machine::calibrate(void){
  this->state = CALIBRATE;
  Poller isDone = Poller(false);

  this->currentState = [this, &isDone](void){
    liftMotors->move(-5);
    if(liftMotors->getVelocity()){
      liftMotors->move(0);
      liftMotors->setZeroPosition();
      isDone.setPoller(true);
    }
  };

  Poller timer = Poller(200);
  return Poller(&timer, &isDone);
};

Poller Lift::Machine::movePower(int power){
  state = LIFT_POWER;
  this->currentState = [this, power](void){
    liftMotors->move(power);
  };
  return liftMotors->move(power);
};

Poller Lift::Machine::grabStack(void){
  this->currentState = [this](void){
    if(liftMotors->getPosition() > INTAKE_POS+DEADBAND){
      liftMotors->moveVelocity(-40, DEADBAND);
    } else {
      liftMotors->move(0);
    }
  };
  return Poller(std::bind(&MotorGroup::getPosition, liftMotors), INTAKE_POS+DEADBAND);
};


void Lift::Machine::handle(void){
  if(liftMotors->getPosition() < INTAKE_POS + DEADBAND && (state == DROP_STACK || state == INTAKE || Input::getLiftPower() < 0)){
    liftMotors->move(holdPower);
  } else {
    this->currentState();
  }
};
