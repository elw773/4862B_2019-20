#include "main.h"

Atn::Auton::Auton(std::function<void(bool)> func, std::string name){
  this->func = func;
  this->name = name;
};

void Atn::Auton::run(bool side){
  this->func(side);
};
