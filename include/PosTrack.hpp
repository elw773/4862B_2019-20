#ifndef _POS_TRACK_HPP_
#define _POS_TRACK_HPP_

#include "api.h"

namespace PosTrack{
  class PosTracker{
    pros::ADIEncoder frontSwEnc; // front sideways encoder
    pros::ADIEncoder midStEnc; // middle straight encoder
    pros::ADIEncoder backSwEnc; // front sideways encoder
  };
}

#endif //_POS_TRACK_HPP_
