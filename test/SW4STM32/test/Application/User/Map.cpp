#include "Map.h"

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  if ((in_max - in_min) > (out_max - out_min)) {
    return (x - in_min) * (out_max - out_min+1) / (in_max - in_min+1) + out_min;
  }
  else
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
}
