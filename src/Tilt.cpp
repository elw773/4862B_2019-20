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

void Tilt::Machine::setState(State state){
  this->state = state;

  switch(state){
    case CALIBRATE: calibrate(); break;
    case STOP: poller = tiltMotors->move(0); break;
    case DROP_STACK: dropStack(); break;
    case LIFT_POWER: poller = Poller(); break;
    case TILT_POWER: movePower(Input::getLiftPower()); break;

    default:
      double pos = stateToPos(state);
      this->currentState = [this, pos](void){
        tiltMotors->movePosition(pos, DEF_VELOCITY, DEADBAND);
      };
      poller = tiltMotors->movePosition(pos, DEF_VELOCITY, DEADBAND);
      break;
  }
};

void Tilt::Machine::movePower(int power){
  state = TILT_POWER;
  this->currentState = [this, power](void){
    tiltMotors->move(power);
  };
  poller = tiltMotors->move(power);
};

void Tilt::Machine::calibrate(void){
  this->state = CALIBRATE;

  this->currentState = [this](void){
    tiltMotors->move(-25);
    if(tiltMotors->getVelocity() == 0){
      tiltMotors->setZeroPosition();
    }
  };
  std::function<bool(int*)> timer = Poller(300).getIsDone();
  poller = Poller([this, timer](int* param){
    if(tiltMotors->getVelocity() == 0 && timer(param)){
      tiltMotors->setZeroPosition();
      tiltMotors->move(0);
      return true;
    }
    return false;
  });
};

void Tilt::Machine::dropStack(void){
  this->state = DROP_STACK;
  this->currentState = [this](void){
      //tiltMotors->movePosition(DROP_STACK_POS, SLOW_VELOCITY, DEADBAND);
      double error = DROP_STACK_POS - tiltMotors->getPosition();
      if(error > DEADBAND){
        int velocity = (int)(error * 0.023);
        if(velocity > SLOW_VELOCITY){
          velocity = SLOW_VELOCITY;
        }
        tiltMotors->moveVelocity(velocity);
      } else {
        tiltMotors->movePosition(DROP_STACK_POS, 50);
      }
  };

  poller = tiltMotors->movePosition(DROP_STACK_POS, SLOW_VELOCITY, DEADBAND+450, 200);
};

void Tilt::Machine::handle(void){
  if(tiltMotors->getPosition() < BOT_INTAKE_POS + DEADBAND + CALIBRATE_OFFSET && state == BOT_INTAKE && state != CALIBRATE){
    tiltMotors->move(BOT_HOLD_POWER);
  }/* else if(tiltMotors->getPosition() > SLOW_POS && state == DROP_STACK){ // if is moving up and it it in slow zone
    tiltMotors->movePosition(DROP_STACK_POS, SLOW_VELOCITY, DEADBAND); // go where you were going, but slower now
  }*/ else {
    this->currentState();

  }
};


Poller* Tilt::Machine::getPoller(){
  return &poller;
};
