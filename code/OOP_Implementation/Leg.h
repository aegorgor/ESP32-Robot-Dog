#ifndef Leg_H
#define Leg_H

#include "RServo.h" 

class Leg 
{
  public:
    RServo aServo;
    RServo fServo;
    RServo tServo;
    Leg(RServo *servo1, RServo *servo2, RServo *servo3);
    void setZ(double desiredHeight);
    void setXZ(double desiredHeight, double forwardDist);
    void setXYZ(double oldLegLength, double forwardDist, double latDist);
    void interpolatePrep(double setRate);
    void moveToNextPosition();
    double getX();
    double getY();
    double getZ();
    double femurAngle;
    
    double x = 0;
    double y = 0;
    double z = 150;
};

#endif
