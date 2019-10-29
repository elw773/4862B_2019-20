#include "main.h"

int Handleable::getState(void){
  return this->state;
};
void Handleable::handle(void){
  this->currentState();
};
