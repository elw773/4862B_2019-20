#include "main.h"

double limit(double val, double limit){
  if(fabs(val) > limit){
    val = limit * sign(val);
  }
  return val;
};

int sign(double x){
  if(x > 0) return 1;
    if(x < 0) return -1;
    return 0;
};

double clipAngle(double a){
  while(a > M_PI + 0.02 || a < -M_PI - 0.02){
    a -= M_PI * sign(a);
  }
  return a;
};


double degreeToRad(double a){
  return a * 0.0174533;
};

double radToDegree(double a){
  return a * 57.2958;
};
