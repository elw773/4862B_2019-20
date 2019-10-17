#ifndef _SUBSYSTEM_HPP_
#define _SUBSYSTEM_HPP_

#include "api.h"
#include "Poller.hpp"
#include "StateMachine.hpp"

class Subsystem {
  pros::Motor motors[8];
  int slew;

  public:
    Subsystem(int motorPorts[], pros::motor_gearset_e_t gearset, int slew);

    Poller moveVoltage(int power);

    Poller moveVelocity(int velocity, int custom1 = INT32_MAX, int custom2 = INT32_MAX, int custom3 = INT32_MAX);

    Poller movePosition(int position, int maxVelocity, int custom1 = INT32_MAX, int custom2 = INT32_MAX, int custom3 = INT32_MAX);

    int getVelocity();

    int getVoltage();

    int getPosition();

    Poller handle();
};

#endif //_SUBSYSTEM_HPP_
