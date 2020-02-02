#include "main.h"

double Tilt::Machine::stateToPos(State state){

  switch(state){
    case TOWER: return TOWER_POS;
    case BOT_INTAKE: return BOT_INTAKE_POS;
    case MID_INTAKE: return MID_INTAKE_POS;
    case HIGH_INTAKE: return HIGH_INTAKE_POS;
    case ALMOST_STACK: return ALMOST_STACK_POS;
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
  if(state == State::TOWER){
    state = State::BOT_INTAKE;
  }

  switch(state){
    case CALIBRATE: calibrate(); break;
    case STOP: movePower(0); break;
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
    tiltMotors->move(-17);
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
        // previously 0.037, 0.047e
        int velocity = (int)(error * 0.05);
        if(velocity > SLOW_VELOCITY){
          velocity = velocity;
        }
        tiltMotors->moveVelocity(velocity);
      } else {
        tiltMotors->movePosition(DROP_STACK_POS, 50);
      }
  };

  poller = tiltMotors->movePosition(DROP_STACK_POS, SLOW_VELOCITY, DEADBAND+450);
};

void Tilt::Machine::handle(void){
  if(tiltMotors->getPosition() < 150 && state == BOT_INTAKE && state != CALIBRATE){
    tiltMotors->move(0);
  }/* else if(tiltMotors->getPosition() > SLOW_POS && state == DROP_STACK){ // if is moving up and it it in slow zone
    tiltMotors->movePosition(DROP_STACK_POS, SLOW_VELOCITY, DEADBAND); // go where you were going, but slower now
  }*/ else {
    this->currentState();

  }
};


Poller* Tilt::Machine::getPoller(){
  return &poller;
};
