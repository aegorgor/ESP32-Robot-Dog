#include "Leg.h" 

//CONSTANTS
#define TIBIA 107 //107.5 acc, 95.53 before change
#define FEMUR 100.4
#define RSPOKE 25
#define LSPOKE 22.58
#define ROD 33.58
#define X 51.29
#define Y 38.71
#define ABDUCTOR_HOR_OFFSET 39.9 //40.45//42.66  //39.20  
#define ABDUCTOR_VER_OFFSET 13.35 //40.45//42.66  //39.20  
#define ABDUCTOR_HYPOTENUSE 42.07
#define ABDUCTOR_LEG_ANGLE 71.74 //72.05//72.12 //70.48

Leg::Leg(RServo *servo1, RServo *servo2, RServo *servo3): aServo(*servo1), fServo(*servo2), tServo(*servo3)
{
}
void Leg::interpolatePrep(double setRate)
{
    this->aServo.servoValChange = (this->aServo.angle - this->aServo.angleOld) / setRate;
    this->fServo.servoValChange = (this->fServo.angle - this->fServo.angleOld) / setRate;
    this->tServo.servoValChange = (this->tServo.angle - this->tServo.angleOld) / setRate;

    this->aServo.servoVal = this->aServo.angleOld;
    this->fServo.servoVal = this->fServo.angleOld;
    this->tServo.servoVal = this->tServo.angleOld;

    this->aServo.angleOld = this->aServo.angle;
    this->fServo.angleOld = this->fServo.angle;
    this->tServo.angleOld = this->tServo.angle;
}
double rad2deg(double rad)
{
  return rad * 57296 / 1000;
}
double deg2rad(double deg)
{
  return deg * 1000 / 57296;
}

void Leg::setZ(double desiredHeight)
{  
  double theta = acos((sq(FEMUR) + sq(TIBIA) - sq(desiredHeight)) / (2 * FEMUR * TIBIA)); //radians
  double femurAngleVal = 90 - rad2deg(asin(sin(theta) * TIBIA / (desiredHeight + 0.0001))); //degrees
  double tibiaAngleVal = 190 - (rad2deg(theta) + femurAngleVal * -1 + Y + X);
  this->fServo.angle = femurAngleVal; 
  this->tServo.angle = tibiaAngleVal;
  this->z = desiredHeight;
}

void Leg::setXZ(double desiredHeight, double forwardDist)
{
  this->setZ(sqrt(sq(desiredHeight) + sq(forwardDist)));
  this->fServo.angle += rad2deg(atan(forwardDist / desiredHeight));
  this->tServo.angle += rad2deg(atan(forwardDist / desiredHeight));
  this->x = forwardDist;
  this->z = desiredHeight;
}

void Leg::setXYZ(double oldLegLength, double forwardDist, double latDist)
{
  latDist += ABDUCTOR_HOR_OFFSET;//39.35;//40.60;// potential issue depending on which way positive displacement goes
  oldLegLength -= ABDUCTOR_VER_OFFSET;
  double abductorAngleVal = rad2deg(atan(latDist / oldLegLength)) - rad2deg(atan(ABDUCTOR_HOR_OFFSET / oldLegLength)); //40.60
  
  double hypotenuse = sqrt(sq(latDist) + sq(oldLegLength));
  double phi = rad2deg(asin(sin(deg2rad(ABDUCTOR_LEG_ANGLE)) * ABDUCTOR_HYPOTENUSE / hypotenuse));
  double newLegLength = hypotenuse * sin(deg2rad(180 - phi - ABDUCTOR_LEG_ANGLE)) / sin(deg2rad(ABDUCTOR_LEG_ANGLE));// - 4.5 * cos(abductorAngleVal);
  Serial.println("abductorAngle Val: ");
  Serial.print(abductorAngleVal);
  Serial.println("new leg length: ");
  Serial.print(newLegLength  - 1.8*abductorAngleVal);
  newLegLength  -= 1.9*abductorAngleVal;

  this->x = forwardDist;
  this->y = latDist;
  this->z = oldLegLength;
  this->aServo.angle = (abductorAngleVal);
  if (forwardDist == 0)
    this->setZ(newLegLength);
  else
    this->setXZ(newLegLength, forwardDist);


}

double Leg::getX(){
  return this->x;
}
double Leg::getY(){
  return this->y;
} 
double Leg::getZ(){
  return this->z;
}

void Leg::moveToNextPosition()
{
  this->aServo.servoVal += this->aServo.servoValChange;
  this->tServo.servoVal += this->tServo.servoValChange;
  this->fServo.servoVal += this->fServo.servoValChange;
  
  this->aServo.controlServo();
  this->tServo.controlServo();
  this->fServo.controlServo();
}
