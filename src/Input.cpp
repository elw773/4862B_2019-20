#include "main.h"

pros::Controller Input::controller(pros::E_CONTROLLER_MASTER);

bool Input::towerBtn = false;

LiftTilt::State Input::getLiftTiltState(void){
  bool topShldr = controller.get_digital(DIGITAL_L1);
  bool botShldr = controller.get_digital(DIGITAL_L2);
  bool topDpad = controller.get_digital(DIGITAL_UP);
  bool botDpad = controller.get_digital(DIGITAL_DOWN);
  bool leftDpad = controller.get_digital(DIGITAL_LEFT);
  bool rightDpad = controller.get_digital(DIGITAL_RIGHT);

  int liftState = Robot::lift.getState();
  int tiltState = Robot::tilt.getState();
  int liftTiltState = Robot::liftTilt.getState();


  if(topDpad){
    return LiftTilt::HIGH_INTAKE;
  }

  if(botDpad){
    return LiftTilt::BOT_INTAKE;
  }

  if(leftDpad){
    return LiftTilt::MID_INTAKE;
  }

  if(rightDpad){
    return LiftTilt::DROP_STACK;
  }


  if(topShldr && !towerBtn){
    switch(liftState){
      case Lift::LOW_TOWER: return LiftTilt::MID_TOWER; break;
      case Lift::MID_TOWER: return LiftTilt::MID_TOWER; break;
      default: return LiftTilt::LOW_TOWER; break;
    }
  }
  if(botShldr && !towerBtn){
    switch(liftState){
      case Lift::MID_TOWER: return LiftTilt::LOW_TOWER; break;
      default: return LiftTilt::MID_INTAKE; break;
    }
  }

  towerBtn = topShldr || botShldr;

  LiftTilt::State state;
  try{
    state = static_cast<LiftTilt::State>(liftTiltState);
  } catch (...){
    state = LiftTilt::STOP;
  }

  return state; // if no new state, use previous state
};

Intake::State Input::getIntakeState(void){
  bool topShldr = controller.get_digital(DIGITAL_R1);
  bool botShldr = controller.get_digital(DIGITAL_R2);

  if(topShldr && botShldr){
    return Intake::HOLD;
  }
  if(topShldr){
    return Intake::INTAKE;
  }
  if(botShldr){
    if(Robot::tilt.getState() == Tilt::DROP_STACK){
      return Intake::STACK_OUTTAKE;
    } else {
      return Intake::OUTTAKE;
    }
  }

  return Intake::STOP;
};

int Input::getRightDrive(void){
  return controller.get_analog(ANALOG_RIGHT_Y) - controller.get_analog(ANALOG_RIGHT_X);
};

int Input::getleftDrive(void){
  return controller.get_analog(ANALOG_RIGHT_Y) + controller.get_analog(ANALOG_RIGHT_X);
};
