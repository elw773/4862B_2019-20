#include "main.h"

LiftTilt::Machine::Machine(Tilt::Machine* tilt, Lift::Machine* lift){
  this->tilt = tilt;
  this->lift = lift;
};

Poller LiftTilt::Machine::setState(State state){
  Poller liftPoller;
  Poller tiltPoller;
  switch(state){
    case ALLIANCE_TOWER:
      liftPoller = lift->setState(Lift::ALLIANCE_TOWER);
      tiltPoller = tilt->setState(Tilt::TOWER);
      break;
    case LOW_TOWER:
      liftPoller = lift->setState(Lift::LOW_TOWER);
      tiltPoller = tilt->setState(Tilt::TOWER);
      break;
    case MID_TOWER:
       liftPoller = lift->setState(Lift::MID_TOWER);
       tiltPoller = tilt->setState(Tilt::TOWER);
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
  }
  return Poller(&liftPoller, &tiltPoller);
};

void LiftTilt::Machine::handle(void){
  lift->handle();
  tilt->handle();
};
