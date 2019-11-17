#include "main.h"

LiftTilt::Machine::Machine(Tilt::Machine* tilt, Lift::Machine* lift){
  this->tilt = tilt;
  this->lift = lift;
};

Poller LiftTilt::Machine::setState(State state){
  this->state = state;

  Poller liftPoller;
  Poller tiltPoller;
  switch(state){
    case PRE_TWO_GRAB:
      liftPoller = lift->setState(Lift::PRE_TWO_GRAB);
      tiltPoller = tilt->setState(Tilt::BOT_INTAKE);
      break;
    case GRAB_STACK:
       liftPoller = lift->setState(Lift::GRAB_STACK);
       tiltPoller = tilt->setState(Tilt::BOT_INTAKE);
       break;
    case LOW_TOWER:
      liftPoller = lift->setState(Lift::LOW_TOWER);
      tiltPoller = tilt->setState(Tilt::TOWER);
      break;
    case MID_TOWER:
       liftPoller = lift->setState(Lift::MID_TOWER);
       tiltPoller = tilt->setState(Tilt::TOWER);
       break;
    case DROP_STACK:
      liftPoller = lift->setState(Lift::DROP_STACK);
      tiltPoller = tilt->setState(Tilt::DROP_STACK);
      break;
    case BOT_INTAKE:
      liftPoller = lift->setState(Lift::INTAKE);
      tiltPoller = tilt->setState(Tilt::BOT_INTAKE);
      break;
    case MID_INTAKE:
      liftPoller = lift->setState(Lift::INTAKE);
      tiltPoller = tilt->setState(Tilt::MID_INTAKE);
      break;
    case HIGH_INTAKE:
      liftPoller = lift->setState(Lift::INTAKE);
      tiltPoller = tilt->setState(Tilt::HIGH_INTAKE);
      break;
    case STOP:
      liftPoller = lift->setState(Lift::STOP);
      tiltPoller = tilt->setState(Tilt::STOP);
      break;
    case CALIBRATE:
      liftPoller = lift->setState(Lift::CALIBRATE);
      tiltPoller = tilt->setState(Tilt::CALIBRATE);
      break;
    case LIFT_POWER:
      liftPoller = lift->setState(Lift::LIFT_POWER);
      tiltPoller = tilt->setState(Tilt::LIFT_POWER);
      break;
  }
  return Poller(liftPoller.getIsDone(), tiltPoller.getIsDone());
};

void LiftTilt::Machine::handle(void){
  lift->handle();
  tilt->handle();
};
