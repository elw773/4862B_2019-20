#include "main.h"

LiftTilt::Machine::Machine(Tilt::Machine* tilt, Lift::Machine* lift){
  this->tilt = tilt;
  this->lift = lift;
};

void LiftTilt::Machine::setState(State state){
  this->state = state;

  Poller liftPoller;
  Poller tiltPoller;
  switch(state){
    case PRE_TWO_GRAB:
      lift->setState(Lift::PRE_TWO_GRAB);
      tilt->setState(Tilt::BOT_INTAKE);
      break;
    case GRAB_STACK:
       lift->setState(Lift::GRAB_STACK);
       tilt->setState(Tilt::BOT_INTAKE);
       break;
    case LOW_TOWER:
      lift->setState(Lift::LOW_TOWER);
      tilt->setState(Tilt::TOWER);
      break;
    case MID_TOWER:
       lift->setState(Lift::MID_TOWER);
       tilt->setState(Tilt::TOWER);
       break;
    case DROP_STACK:
      lift->setState(Lift::DROP_STACK);
      Poller(true);
      tilt->setState(Tilt::DROP_STACK);
      break;
    case BOT_INTAKE:
      lift->setState(Lift::INTAKE);
      tilt->setState(Tilt::BOT_INTAKE);
      break;
    case MID_INTAKE:
      lift->setState(Lift::INTAKE);
      tilt->setState(Tilt::MID_INTAKE);
      break;
    case HIGH_INTAKE:
      lift->setState(Lift::INTAKE);
      tilt->setState(Tilt::HIGH_INTAKE);
      break;
    case STOP:
      lift->setState(Lift::STOP);
      tilt->setState(Tilt::STOP);
      break;
    case CALIBRATE:
      lift->setState(Lift::CALIBRATE);
      tilt->setState(Tilt::CALIBRATE);
      break;
    case LIFT_POWER:
      lift->setState(Lift::LIFT_POWER);
      tilt->setState(Tilt::LIFT_POWER);
      break;
    case TILT_POWER:
      lift->setState(Lift::TILT_POWER);
      tilt->setState(Tilt::TILT_POWER);
      break;
  }
  poller = Poller(lift->getPoller()->getIsDone(), tilt->getPoller()->getIsDone());
};

void LiftTilt::Machine::handle(void){
  lift->handle();
  tilt->handle();
};



Poller* LiftTilt::Machine::getPoller(){
  return &poller;
};
