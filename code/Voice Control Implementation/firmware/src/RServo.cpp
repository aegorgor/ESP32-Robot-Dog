#include "RServo.h"
#include <Wire.h>

Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40);

void RServo::controlServo(double servoVal)
{
  if(this->minOrMax == 1)
    pca9685.setPWM(this->servoNum, 0, SERVOMAX2 - (this->servoVal + this->defaultOffset) * this->servoRatio);
  else
    pca9685.setPWM(this->servoNum, 0, SERVOMIN2 + (this->servoVal + this->defaultOffset) * this->servoRatio);
}

void RServo::controlServo()
{
  this->controlServo(this->servoVal);
}

RServo::RServo(int minOrMax, double servoNum, double servoRatio, double defaultOffset)
{
  this->minOrMax = minOrMax;
  this->servoNum = servoNum;
  this->servoRatio = servoRatio;
  this->defaultOffset = defaultOffset;
  this->angle = 0;
  this->angleOld = 0;
  this->servoVal = 0;
  this->servoValChange = 0;
}
