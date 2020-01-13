#include "main.h"

int Lift::holdPower = Lift::INTAKE_HOLD_POWER;

double Lift::Machine::stateToPos(State state){
  switch(state){
    case DEPLOY: return DEPLOY_POS;
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

void Lift::Machine::setState(State state){
  this->state = state;
  switch(state){
    case CALIBRATE: calibrate(); return;
    case STOP: movePower(0); return;
    case GRAB_STACK: grabStack(); return;
    case LIFT_POWER: movePower(Input::getLiftPower()); return;
    case INTAKE: holdPower = INTAKE_HOLD_POWER; break;
    case TILT_POWER: movePower(0); return;
    case DROP_STACK: movePower(DROP_STACK_HOLD_POWER); return;
    default: break;
  }
  double pos = stateToPos(state);
  this->currentState = [this, pos](void){
    liftMotors->movePosition(pos, DEF_VELOCITY, DEADBAND);
  };
  poller = liftMotors->movePosition(pos, DEF_VELOCITY, DEADBAND);
};


void Lift::Machine::calibrate(void){
  this->state = CALIBRATE;

  this->currentState = [this](void){
    liftMotors->move(-25);
    if(liftMotors->getVelocity() == 0){
      liftMotors->setZeroPosition();
    }
  };

  std::function<bool(int*)> timer = Poller(300).getIsDone();
  poller = Poller([this, timer](int* param){
    if(liftMotors->getVelocity() == 0 && timer(param)){
      liftMotors->setZeroPosition();
      liftMotors->move(0);
      return true;
    }
    return false;
  });
};

void Lift::Machine::movePower(int power){
  state = LIFT_POWER;
  this->currentState = [this, power](void){
    liftMotors->move(power);
  };
  poller = liftMotors->move(power);
};

void Lift::Machine::grabStack(void){
  this->currentState = [this](void){
    if(liftMotors->getPosition() > INTAKE_POS+DEADBAND){
      liftMotors->moveVelocity(-40, DEADBAND);
    } else {
      liftMotors->move(0);
    }
  };
  poller = Poller(std::bind(&MotorGroup::getPosition, liftMotors), INTAKE_POS+DEADBAND);
};


void Lift::Machine::handle(void){
  if(liftMotors->getPosition() < INTAKE_POS + DEADBAND + CALIBRATE_OFFSET && (state == DROP_STACK || state == INTAKE || Input::getLiftPower() < 0) && state != CALIBRATE){
    liftMotors->move(holdPower);
  } else {
    this->currentState();
  }
};


Poller* Lift::Machine::getPoller(){
  return &poller;
};
