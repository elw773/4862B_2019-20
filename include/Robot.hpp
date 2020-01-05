#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "api.h"

#include "Handleable.hpp"
#include "Poller.hpp"
#include "MotorGroup.hpp"

#include "Drive.hpp"
#include "Lift.hpp"
#include "Tilt.hpp"
#include "Intake.hpp"
#include "LiftTilt.hpp"

namespace Robot {
  extern const int numMotors;

  extern std::vector<pros::Motor*> motors;
  extern std::vector<std::string> motorNames;

  extern MotorGroup intakeMotorGroup;
  extern MotorGroup liftMotorGroup;
  extern MotorGroup tiltMotorGroup;
  extern MotorGroup leftMotorGroup;
  extern MotorGroup rightMotorGroup;

  extern Intake::Machine intake;

  extern Lift::Machine lift;
  extern Tilt::Machine tilt;
  extern LiftTilt::Machine liftTilt;

  extern Drive::Machine drive;

  extern PosTrack::PosTracker posTracker;

  extern std::vector<Handleable*> machines;

  void handle(void);

  void stop(void);

  void wait(Poller* poller);
};

#endif //_ROBOT_HPP_
