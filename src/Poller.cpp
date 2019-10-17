#include "main.h"

Poller::Poller(std::function<bool(int*)> isDone){
  this->isDone = isDone;
};

Poller::Poller(std::function<double(void)> value, double target, double range, int timeout = 0){
  this->isDone = [value, target, range, timeout](int* timeInTarget){
      if(fabs(target-value()) < range){
        if(*timeInTarget == NOT_IN_TARGET){ // if was not in target
          *timeInTarget = pros::millis();
        }

        if((pros::millis() - *timeInTarget) > timeout){ // if was in target loger than timeout
          return true;
        }
      } else {
        *timeInTarget = NOT_IN_TARGET;
      }
      return false;
  };
};

Poller::Poller(std::function<double(void)> value, double target){
  if(value() < target){ // if value starts below target,
    this->isDone = [value, target](int* timeInTarget){
      return value() > target; //true if value greater than target
    };
  } else if(value() > target){ // if value starts above target
    this->isDone = [value, target](int* timeInTarget){
      return value() < target; //true if value less than target
    };
  } else { // if they start as the same value
    this->isDone = [value, target](int* timeInTarget){
      return true;
    };
  }
};

bool Poller::finished(void){
  return isDone(&timeInTarget);
};
