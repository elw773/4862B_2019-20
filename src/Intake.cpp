#include "main.h"

Intake::Machine::Machine(MotorGroup* intakeMotors){
  this->intakeMotors = intakeMotors;
};

void Intake::Machine::setState(State state){
  if(state == State::PREP_CUBE || state == State::OUTTAKE_CUBE){
    int goal = intakeMotors->getPosition() - (state==State::PREP_CUBE?500:2000);
    poller = Poller(false);
    this->currentState = [goal, this](void){
      intakeMotors->movePosition(goal);
      if(abs(goal - intakeMotors->getPosition()) < 100){
        poller.setPoller(true);
      }
    };
  } else {
    int power = stateToPower(state);
    this->state = state;

    this->currentState = [power, this](void){
      intakeMotors->move(power);
    };
    poller = Poller();
  }
}


Poller* Intake::Machine::getPoller(){
  return &poller;
};


int Intake::Machine::stateToPower(State state){
  switch(state){
    case STACK_OUTTAKE: return STACK_OUTTAKE_POWER;
    case OUTTAKE: return OUTTAKE_POWER;
    case STOP: return STOP_POWER;
    case HOLD: return HOLD_POWER;
    case GRAB_STACK: return GRAB_STACK_POWER;
    case INTAKE: return INTAKE_POWER;
    default: return 0;
  }
}
