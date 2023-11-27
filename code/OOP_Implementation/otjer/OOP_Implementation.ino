#include "Leg.h" 
#include "OOP_Implementation.h" 
#include "Gait_planning_and_execution.cpp"
#include <Wire.h>

int counter = 1;
const int SERVOMIN2 = 66;
const int SERVOMAX2 = 584;
    
void setup() 
{
  pca9685.begin();
  pca9685.setPWMFreq(50);
  Serial.begin(9600);
  leg0.setZ(170);
  leg1.setZ(170);
  leg2.setZ(170);
  leg3.setZ(170);
  interpolate("0123", 500000);
}

void loop() 
{
  int walkCycle = 300000;
  turningWalkingSequenceClockwise(600000);
}
void straightWalkingSequence(double walkCycleTotal)
{
  int stepLength = 30;
  double walkCycleSub = walkCycleTotal / 8;
  
  leg0.setXZ(155, 0); 
  leg2.setXZ(155, 0);
  leg1.setXZ(170, -10); //Mid point
  leg3.setXZ(170, -10);
  interpolate("0123", walkCycleSub);
  leg0.setXZ(155, 20); 
  leg2.setXZ(155, 20);
  leg1.setXZ(170, -20); //Mid point
  leg3.setXZ(170, -20);
  interpolate("0123", walkCycleSub);
  leg0.setXZ(155, 40); 
  leg2.setXZ(155, 40);
  leg1.setXZ(170, -30); //Mid point
  leg3.setXZ(170, -30);
  interpolate("0123", walkCycleSub);
  leg0.setXZ(170, 40); 
  leg2.setXZ(170, 40);
  leg1.setXZ(170, -40); //Mid point
  leg3.setXZ(170, -40);
  interpolate("0123", walkCycleSub);


  leg1.setXZ(155, 0); 
  leg3.setXZ(155, 0);
  leg0.setXZ(170, -10); //Mid point
  leg2.setXZ(170, -10);
  interpolate("0123", walkCycleSub);
  leg1.setXZ(155, 20); 
  leg3.setXZ(155, 20);
  leg0.setXZ(170, -20); //Mid point
  leg2.setXZ(170, -20);
  interpolate("0123", walkCycleSub);
  leg1.setXZ(155, 40); 
  leg3.setXZ(155, 40);
  leg0.setXZ(170, -30); //Mid point
  leg2.setXZ(170, -30);
  interpolate("0123", walkCycleSub);
  leg1.setXZ(170, 40); 
  leg3.setXZ(170, 40);
  leg0.setXZ(170, -40); //Mid point
  leg2.setXZ(170, -40);
  interpolate("0123", walkCycleSub);
}
void turningWalkingSequenceCounterClockwise(double walkCycleTotal)
{
  double walkCycleSub = walkCycleTotal / 5;

  leg0.setXYZ(170, 0, 45); //opp legs push away
  leg2.setXYZ(170, 0, 45);
  leg1.setXYZ(140, 0, 0); //other legs raise
  leg3.setXYZ(140, 0, 0);
  interpolate("0123", walkCycleSub);
  leg0.setXYZ(170, 0, 90);
  leg2.setXYZ(170, 0, 90);
  leg1.setXYZ(160, 0, 0);
  leg3.setXYZ(160, 0, 0);
  interpolate("0123", walkCycleSub);
  
  leg0.setXYZ(140, 0, 90); //all legs brought to starting position
  leg2.setXYZ(140, 0, 90);
  leg1.setXYZ(170, 0, 0);
  leg3.setXYZ(170, 0, 0);
  interpolate("0123", (int)(walkCycleSub*0.7));
  leg0.setXYZ(140, 0, 0);
  leg2.setXYZ(140, 0, 0);
  leg1.setXYZ(170, 0, 0);
  leg3.setXYZ(170, 0, 0);
  interpolate("0123", (int)(walkCycleSub*0.7));
  leg0.setXYZ(170, 0, 0);
  leg2.setXYZ(170, 0, 0);
  leg1.setXYZ(160, 0, 0);
  leg3.setXYZ(160, 0, 0);
  interpolate("0123", (int)(walkCycleSub*0.7));
}
void turningWalkingSequenceClockwise(double walkCycleTotal)
{
  double walkCycleSub = walkCycleTotal / 5;

  leg1.setXYZ(170, 0, 45); //opp legs push away
  leg3.setXYZ(170, 0, 45);
  leg0.setXYZ(140, 0, 0); //other legs raise
  leg2.setXYZ(140, 0, 0);
  interpolate("0123", walkCycleSub);
  leg1.setXYZ(170, 0, 90);
  leg3.setXYZ(170, 0, 90);
  leg0.setXYZ(160, 0, 0);
  leg2.setXYZ(160, 0, 0);
  interpolate("0123", walkCycleSub);
  
  leg1.setXYZ(140, 0, 90); //all legs brought to starting position
  leg3.setXYZ(140, 0, 90);
  leg0.setXYZ(170, 0, 0);
  leg2.setXYZ(170, 0, 0);
  interpolate("0123", (int)(walkCycleSub*0.7));
  leg1.setXYZ(140, 0, 0);
  leg3.setXYZ(140, 0, 0);
  leg0.setXYZ(170, 0, 0);
  leg2.setXYZ(170, 0, 0);
  interpolate("0123", (int)(walkCycleSub*0.7));
  leg1.setXYZ(170, 0, 0);
  leg3.setXYZ(170, 0, 0);
  leg0.setXYZ(160, 0, 0);
  leg2.setXYZ(160, 0, 0);
  interpolate("0123", (int)(walkCycleSub*0.7));
}


void interpolate(String legs, double walkCycle)
{
  double setRate = walkCycle / 20000;

  //ESTABLISHING THE CHANGE RATES FOR EVERY VARIABLE
  if (legs.charAt(0) == '0')
    leg0.interpolatePrep(setRate);
  if (legs.charAt(1) == '1')
    leg1.interpolatePrep(setRate);
  if (legs.charAt(2) == '2')
      leg2.interpolatePrep(setRate);
  if (legs.charAt(3) == '3')
    leg3.interpolatePrep(setRate);

  //CONTROLLING EVERY SERVO
  for (int i = 0; i < setRate; i++)
  {
    if (legs.charAt(0) == '0')
    {
      leg0.moveToNextPosition(); 
    }
    if (legs.charAt(1) == '1')
    {
      leg1.moveToNextPosition(); 
    }
    if (legs.charAt(2) == '2')
    {
       leg2.moveToNextPosition(); 
    }
    if (legs.charAt(3) == '3')
    {
      leg3.moveToNextPosition(); 
    }
    delayMicroseconds(walkCycle / setRate);
  }
}
