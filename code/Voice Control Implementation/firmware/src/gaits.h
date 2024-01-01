#ifndef GAITS_H
#define GAITS_H

#include "Leg.h" 

class gaits
{
  public:    
    static const int SERVOMIN2 = 66;
    static const int SERVOMAX2 = 584;
    static void interpolate(String legs, double walkCycle);
    static void straightWalkingSequence(double walkCycleTotal);
    static void straightWalkingSequenceRear(double walkCycleTotal);
    static void turningWalkingSequenceCounterClockwise(double walkCycleTotal);
    static void turningWalkingSequenceClockwise(double walkCycleTotal);
};

#endif // GAITS_H
