#include "main.h"

pros::Controller Input::controller(pros::E_CONTROLLER_MASTER);

bool Input::shldrBtn = false;

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
    return LiftTilt::MID_TOWER;
  }

  if(rightDpad){
    return LiftTilt::DROP_STACK;
  }



  if(!shldrBtn){
    if(topShldr){
      switch(liftTiltState){
        case LiftTilt::MID_TOWER: return LiftTilt::MID_TOWER;
        case LiftTilt::LOW_TOWER: return LiftTilt::MID_TOWER;
        default: return LiftTilt::LOW_TOWER;
      }
    } else if(botShldr){
      switch(liftTiltState){
        case LiftTilt::MID_TOWER: return LiftTilt::LOW_TOWER;
        default: return LiftTilt::BOT_INTAKE;
      }
    }
  }

  shldrBtn = topShldr || botShldr;

 // if no new state, use previous state

  LiftTilt::State lastState;
  try{
    lastState = static_cast<LiftTilt::State>(liftTiltState);
  } catch (...){
    lastState = LiftTilt::STOP;
  }

  return lastState;
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
