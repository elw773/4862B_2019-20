#include "main.h"

Poller::Poller(void){
  this->isDone = [](int* timeInTarget){
    return false;
  };
};

Poller::Poller(bool b){
  this->isDone = [b](int* timeInTarget){
    return b;
  };
};

void Poller::setPoller(bool b){
  this->isDone = [b](int* timeInTarget){
    return b;
  };
};

Poller::Poller(int time){
  this->timeInTarget = pros::millis();
  this->isDone = [time](int* timeInTarget){
    return (pros::millis() - *timeInTarget) > time; // return true if elapsed time is greater than desired time
  };
};

Poller::Poller(std::function<bool(int*)> isDone){
  this->isDone = isDone;
};

Poller::Poller(std::function<double(void)> value, double target, double range, int timeout){
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

Poller::Poller(Poller* a, Poller* b){
  this->isDone = [a, b](int* timeInTarget){
    return a->finished() && b->finished();
  };
}

bool Poller::finished(void){
  return isDone(&timeInTarget);
};
