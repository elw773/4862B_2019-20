#ifndef _INPUT_HPP_
#define _INPUT_HPP_

#include "api.h"
#include "LiftTilt.hpp"
#include "Intake.hpp"

namespace Input{
  extern pros::Controller controller;

  LiftTilt::State getLiftTiltState(void);

  Intake::State getIntakeState(void);

  int getRightDrive(void);

  int getleftDrive(void);
}

#endif //_INPUT_HPP_
