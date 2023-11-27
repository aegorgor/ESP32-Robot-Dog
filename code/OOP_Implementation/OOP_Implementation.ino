#include "gaits.h"
#include "Legs_and_servos.h"
#include <Wire.h>

RServo aServo0(1, 0, 1.5778, 98);
RServo fServo0(0, 2, 2.4079, 45);
RServo tServo0(0, 1, 2.4079, 11);

RServo aServo1(0, 4, 1.5778, 90.6);
RServo fServo1(0, 6, 2.4079, 40);
RServo tServo1(0, 5, 2.34, 10);

RServo aServo2(1, 8, 1.5778, 91.3);
RServo fServo2(1, 10, 2.3544, 49);
RServo tServo2(1, 9, 2.41, 5);

RServo aServo3(0, 12, 1.6333, 94); //1.6333 ratio originally
RServo fServo3(1, 14, 2.3276, 46);
RServo tServo3(1, 13, 2.35, 5);

Leg leg0(&aServo0, &fServo0, &tServo0);
Leg leg1(&aServo1, &fServo1, &tServo1);
Leg leg2(&aServo2, &fServo2, &tServo2);
Leg leg3(&aServo3, &fServo3, &tServo3);

void setup() 
{
  pca9685.begin();
  pca9685.setPWMFreq(50);
  Serial.begin(9600);
  leg0.setZ(170);
  leg1.setZ(170);
  leg2.setZ(170);
  leg3.setZ(170);
  gaits::interpolate("0123", 500000);
}

void loop() 
{
  int walkCycle = 300000;
  while(millis() < 15000)
    gaits::straightWalkingSequence(600000);
  gaits::turningWalkingSequenceClockwise(600000);
}
