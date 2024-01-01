#ifndef RServo
#define RServo_H

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
//#include "Adafruit_PWMServoDriver.h"
extern Adafruit_PWMServoDriver pca9685;

class RServo 
{
  public:
    RServo(int minOrMax, double servoNum, double servoRatio, double defaultOffset);
    int minOrMax;
    double servoNum;
    double servoRatio;
    double defaultOffset;
    
    double angle;
    double angleOld;
    double servoVal;
    double servoValChange;

    void controlServo(double servoVal);
    void controlServo();
    const int SERVOMIN2 = 66;
    const int SERVOMAX2 = 584;
};

#endif
