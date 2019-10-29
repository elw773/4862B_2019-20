#include "main.h"

pros::Controller Input::controller(pros::E_CONTROLLER_MASTER);

bool Input::tiltBtn = false;

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
    return LiftTilt::MID_TOWER;
  }

  if(botDpad){
    return LiftTilt::ALLIANCE_TOWER;
  }

  if(leftDpad){
    return LiftTilt::LOW_TOWER;
  }

  if(rightDpad){
    return LiftTilt::MID_INTAKE;
  }

  if(tiltState == Tilt::TOWER){
    if(topShldr){
      return LiftTilt::HIGH_INTAKE;
    }
    if(botShldr){
      return LiftTilt::BOT_INTAKE;
    }
  } else {
    //toggle through tilter positions
    int nextState = liftTiltState;
    if(!tiltBtn){
      if(topShldr){
        nextState ++;

      } else if(botShldr){
        nextState --;

      }
    }
    tiltBtn = topShldr || botShldr;



    if(nextState > LiftTilt::DROP_STACK){
      nextState = LiftTilt::DROP_STACK;
    } else if(nextState < LiftTilt::BOT_INTAKE){
      nextState = LiftTilt::BOT_INTAKE;
    }


    LiftTilt::State state;
    try{
      state = static_cast<LiftTilt::State>(nextState);
    } catch (...){
      state = LiftTilt::STOP;
    }

    return state;
  }

  // if not going to a tilt-specific state, use d-Dpad



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
