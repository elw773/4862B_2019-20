#ifndef _DRIVE_MACHINE_HPP_
#define _DRIVE_MACHINE_HPP_

#include "api.h"
#include "MotorGroup.hpp"
#include "Poller.hpp"
#include "Handleable.hpp"

namespace Drive {
  enum State{
    POWER
  };

  class Machine : public Handleable{
    MotorGroup* leftMotors;
    MotorGroup* rightMotors;
  public:
    Machine(MotorGroup* leftMotors, MotorGroup* rightMotors);

    Poller move(int left, int right);
  };
};

#endif //_DRIVE_MACHINE_HPP_
