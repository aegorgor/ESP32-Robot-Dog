#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40);

#define aSER0  0   //Servo Motor 0 on connector 0
#define tSER0  1   //Servo Motor 0 on connector 0 
#define fSER0  2   //Servo Motor 0 on connector 0

#define aSER1  4   //Servo Motor 0 on connector 0
#define tSER1  5   //Servo Motor 0 on connector 0
#define fSER1  6   //Servo Motor 0 on connector 0

#define aSER2  8   //Servo Motor 0 on connector 0
#define tSER2  9   //Servo Motor 0 on connector 0
#define fSER2  10  //Servo Motor 0 on connector 0

#define aSER3  12  //Servo Motor 0 on connector 0
#define tSER3  13 //Servo Motor 0 on connector 0
#define fSER3  14 //Servo Motor 0 on connector 0

//CONSTANTS
#define TIBIA 107 //107.5 acc, 95.53 before change
#define FEMUR 100.4
#define RSPOKE 25
#define LSPOKE 22.58
#define ROD 33.58
#define X 51.29
#define Y 38.71
#define ABDUCTOR_OFFSET 40.45//42.66  //39.20
#define ABDUCTOR_LEG_ANGLE 72.05//72.12 //70.48


#define SERVOMIN2  66
#define SERVOMAX2  584
#define fRATIO0 2.4079
#define fRATIO1 2.4079
#define fRATIO2 2.3544
#define fRATIO3 2.3276

#define tRATIO0 2.4079
#define tRATIO1 2.34
#define tRATIO2 2.41
#define tRATIO3 2.35

#define SERVOMIN  78
#define SERVOMAX  557
#define RATIO 2.22477
#define RATIO2 2.31 //for t0 and f3
#define VERTICALABDUCTOR02 230
#define KNEEMOVE 111 //50 degress of movement


double femurAngle0 = 47;
double tibiaAngle0 = 0;
double abductorAngle0 = 0;

double femurAngle1 = 45;
double tibiaAngle1 = 0;
double abductorAngle1 = 0;

double femurAngle2 = 48;
double tibiaAngle2 = 0;
double abductorAngle2 = 0;

double femurAngle3 = 47;
double tibiaAngle3 = 0;
double abductorAngle3 = 0;


double femurAngleOld0 = 47;
double tibiaAngleOld0 = 0;
double abductorAngleOld0 = 0;

double femurAngleOld1 = 45;
double tibiaAngleOld1 = 0;
double abductorAngleOld1 = 0;

double femurAngleOld2 = 48;
double tibiaAngleOld2 = 0;
double abductorAngleOld2 = 0;

double femurAngleOld3 = 47;
double tibiaAngleOld3 = 0;
double abductorAngleOld3 = 0;


double fServoVal0;
double tServoVal0;
double aServoVal0;

double fServoVal1;
double tServoVal1;
double aServoVal1;

double fServoVal2;
double tServoVal2;
double aServoVal2;

double fServoVal3;
double tServoVal3;
double aServoVal3;

double abductorAngleVal;
double femurAngleVal;
double tibiaAngleVal;

int counter = 1;


void legHeight(double desiredHeight, int leg);
void straightWalkingSequence();
double rad2deg(double rad);
double deg2rad(double deg);
void defaultPosition();
void stepForth(double desiredHeight, double forwardDist, int leg);
void sideStep(double latDist, double forwardDist, double legHeight, int leg);
void interpolate(String legs);
void verticalAbductors();
void turningWalkingSequence(double walkCycle);
void turningWalkingSequenceClockwise(double walkCycle);
void turningWalkingSequenceCounterClockwise(double walkCycle);

void setup() {
  Serial.begin(115200);
  Serial.println("PCA9685 Servo Test");
  pca9685.begin();
  pca9685.setPWMFreq(50);
  //legHeight(160, 0);
  //legHeight(160, 1);
  //legHeight(160, 2);
  //legHeight(160, 3);
  //interpolate("0123");
  
}

void loop() {
  int walkCycle = 200000;
//  straightWalkingSequence(walkCycle);
  //
  //Clockwise(walkCycle);
  //turningWalkingSequenceCounterClockwise(walkCycle);
      //pca9685.setPWM(fSER2, 0, SERVOMAX2 - (0 + 48) * fRATIO2);
    //pca9685.setPWM(tSER2, 0, SERVOMAX2 - 13 - 0 * tRATIO2);
    //pca9685.setPWM(aSER2, 0, SERVOMIN + (74.61 + 0) * RATIO);
    
      pca9685.setPWM(fSER3, 0, SERVOMAX2 - (0 + 47) * fRATIO3);
      pca9685.setPWM(tSER3, 0, SERVOMAX2 - 13 - 0 * tRATIO3);
      pca9685.setPWM(aSER3, 0, SERVOMAX - (45.40 + 0) * RATIO); 
  
}
void turningWalkingSequenceCounterClockwise(double walkCycle)
{
  sideStep(170, 0, 25, 1); //opp legs push away
  sideStep(170, 0, 25, 3);
  sideStep(130, 0, 0, 0); //other legs raise
  sideStep(130, 0, 0, 2);  
  interpolate("0123", walkCycle);
  sideStep(170, 0, 50, 1);
  sideStep(170, 0, 50, 3);
  sideStep(170, 0, 0, 0);
  sideStep(170, 0, 0, 2);  
  interpolate("0123", walkCycle);
  
  sideStep(130, 0, 25, 1); //all legs brought to starting position
  sideStep(130, 0, 25, 3);    
  interpolate("0123", walkCycle);
  sideStep(170, 0, 0, 1);
  sideStep(170, 0, 0, 3);   
  interpolate("0123", walkCycle);
}
void turningWalkingSequenceClockwise(double walkCycle)
{
  sideStep(170, 0, 25, 0); //opp legs push away
  sideStep(170, 0, 25, 2);
  sideStep(130, 0, 0, 1); //other legs raise
  sideStep(130, 0, 0, 3);  
  interpolate("0123", walkCycle);
  sideStep(170, 0, 50, 0);
  sideStep(170, 0, 50, 2);
  sideStep(170, 0, 0, 1);
  sideStep(170, 0, 0, 3);  
  interpolate("0123", walkCycle);
  
  sideStep(130, 0, 25, 0); //all legs brought to starting position
  sideStep(130, 0, 25, 2);    
  interpolate("0123", walkCycle);
  sideStep(170, 0, 0, 0);
  sideStep(170, 0, 0, 2);   
  interpolate("0123", walkCycle);
}
void turningWalkingSequence(double walkCycle)
{
  sideStep(30, 0, 170, 0);
  sideStep(30, 0, 170, 2);
  sideStep(30, 0, 140, 1);
  sideStep(30, 0, 140, 3);
  interpolate("0123", walkCycle);
  sideStep(60, 0, 170, 0);
  sideStep(60, 0, 170, 2);
  sideStep(60, 0, 170, 1);
  sideStep(60, 0, 170, 3);
  interpolate("0123", walkCycle); 

  sideStep(30, 0, 170, 1);
  sideStep(30, 0, 170, 3);
  sideStep(30, 0, 140, 0);
  sideStep(30, 0, 140, 2);
  interpolate("0123", walkCycle);
  sideStep(60, 0, 170, 1);
  sideStep(60, 0, 170, 3);
  sideStep(60, 0, 170, 0);
  sideStep(60, 0, 170, 2);
  interpolate("0123", walkCycle); 
}


void straightWalkingSequence()
{
  stepForth(140, 15, 0); //Position B
  stepForth(140, 15, 2);
  stepForth(170, -15, 1); //MID point betwreen C and A
  stepForth(170, -15, 3);
  interpolate("0123");
  stepForth(170, 30, 0); //Position B
  stepForth(170, 30, 2);
  stepForth(170, -30, 1); //
  stepForth(170, -30, 3);
  interpolate("0123");
  //delay(3000);

  stepForth(140, 15, 1); //Position B
  stepForth(140, 15, 3);
  stepForth(170, -15, 0); //MID point betwreen C and A
  stepForth(170, -15, 2);
  interpolate("0123");
  stepForth(170, 30, 1); //Position B
  stepForth(170, 30, 3);
  stepForth(170, -30, 0); //
  stepForth(170, -30, 2);
  interpolate("0123");
  //delay(3000);
}
void interpolate(String legs, double walkCycle)
{
  double fServoValChange0;
  double tServoValChange0;
  double aServoValChange0;
  double fServoValChange1;
  double tServoValChange1;
  double aServoValChange1;
  double fServoValChange2;
  double tServoValChange2;
  double aServoValChange2;
  double fServoValChange3;
  double tServoValChange3;
  double aServoValChange3;

  double setRate = walkCycle/20000;

  //ESTABLISHING THE CHANGE RATES FOR EVERY VARIABLE
  if (legs.charAt(0) == '0')
  {
    fServoValChange0 = (femurAngle0 - femurAngleOld0) / setRate;
    tServoValChange0 = (tibiaAngle0 - tibiaAngleOld0) / setRate;
    aServoValChange0 = (abductorAngle0 - abductorAngleOld0) / setRate;

    fServoVal0 = femurAngleOld0;
    tServoVal0 = tibiaAngleOld0;
    aServoVal0 = abductorAngleOld0;

    femurAngleOld0 = femurAngle0;
    tibiaAngleOld0 = tibiaAngle0;
    abductorAngleOld0 = abductorAngle0;
  }
  if (legs.charAt(1) == '1')
  {
    fServoValChange1 = (femurAngle1 - femurAngleOld1) / setRate;
    tServoValChange1 = (tibiaAngle1 - tibiaAngleOld1) / setRate;
    aServoValChange1 = (abductorAngle1 - abductorAngleOld1) / setRate;

    fServoVal1 = femurAngleOld1;
    tServoVal1 = tibiaAngleOld1;
    aServoVal1 = abductorAngleOld1;

    femurAngleOld1 = femurAngle1;
    tibiaAngleOld1 = tibiaAngle1;
    abductorAngleOld1 = abductorAngle1;
  }
  if (legs.charAt(2) == '2')
  {
    fServoValChange2 = (femurAngle2 - femurAngleOld2) / setRate;
    tServoValChange2 = (tibiaAngle2 - tibiaAngleOld2) / setRate;
    aServoValChange2 = (abductorAngle2 - abductorAngleOld2) / setRate;

    fServoVal2 = femurAngleOld2;
    tServoVal2 = tibiaAngleOld2;
    aServoVal2 = abductorAngleOld2;

    femurAngleOld2 = femurAngle2;
    tibiaAngleOld2 = tibiaAngle2;
    abductorAngleOld2 = abductorAngle2;
  }
  if (legs.charAt(3) == '3')
  {
    fServoValChange3 = (femurAngle3 - femurAngleOld3) / setRate;
    tServoValChange3 = (tibiaAngle3 - tibiaAngleOld3) / setRate;
    aServoValChange3 = (abductorAngle3 - abductorAngleOld3) / setRate;

    fServoVal3 = femurAngleOld3;
    tServoVal3 = tibiaAngleOld3;
    aServoVal3 = abductorAngleOld3;

    femurAngleOld3 = femurAngle3;
    tibiaAngleOld3 = tibiaAngle3;
    abductorAngleOld3 = abductorAngle3;
  }

  //CONTROLLING EVERY SERVO
  for (int i = 0; i < setRate; i++)
  {
    if (legs.charAt(0) == '0')
    {
      fServoVal0 = fServoVal0 + fServoValChange0;
      tServoVal0 = tServoVal0 + tServoValChange0;
      aServoVal0 = aServoVal0 + aServoValChange0;
 
      pca9685.setPWM(fSER0, 0, SERVOMIN2 + (fServoVal0 + 47) * fRATIO0);
      pca9685.setPWM(tSER0, 0, SERVOMIN2 + 20 + tServoVal0 * tRATIO0);      
      pca9685.setPWM(aSER0, 0, SERVOMIN + (aServoVal0 + 68.32) * RATIO);
    }
    if (legs.charAt(1) == '1')
    {
      fServoVal1 = fServoVal1 + fServoValChange1;
      tServoVal1 = tServoVal1 + tServoValChange1;
      aServoVal1 = aServoVal1 + aServoValChange1;

      pca9685.setPWM(fSER1, 0, SERVOMIN2 + (fServoVal1 + 45) * fRATIO1);
      pca9685.setPWM(tSER1, 0, SERVOMIN2 + 15 + tServoVal1 * tRATIO1);
      pca9685.setPWM(aSER1, 0, SERVOMAX - (42.26 + aServoVal1) * RATIO);
    }
    if (legs.charAt(2) == '2')
    {
      fServoVal2 = fServoVal2 + fServoValChange2;
      tServoVal2 = tServoVal2 + tServoValChange2;
      aServoVal2 = aServoVal2 + aServoValChange2;

      pca9685.setPWM(fSER2, 0, SERVOMAX2 - (fServoVal2 + 48) * fRATIO2);
      pca9685.setPWM(tSER2, 0, SERVOMAX2 - 13 - tServoVal2 * tRATIO2);
      pca9685.setPWM(aSER2, 0, SERVOMIN + (74.61 + aServoVal2) * RATIO);
    }
    if (legs.charAt(3) == '3')
    {
      fServoVal3 = fServoVal3 + fServoValChange3;
      tServoVal3 = tServoVal3 + tServoValChange3;
      aServoVal3 = aServoVal3 + aServoValChange3;

      pca9685.setPWM(fSER3, 0, SERVOMAX2 - (fServoVal3 + 47) * fRATIO3);
      pca9685.setPWM(tSER3, 0, SERVOMAX2 - 13 - tServoVal3 * tRATIO3);
      pca9685.setPWM(aSER3, 0, SERVOMAX - (45.40 + aServoVal3) * RATIO);
    }
    delayMicroseconds(walkCycle / setRate);
  }
}


void legHeight(double desiredHeight, int leg)
{
  double theta = acos((sq(FEMUR) + sq(TIBIA) - sq(desiredHeight)) / (2 * FEMUR * TIBIA)); //radians
  femurAngleVal = 90 - rad2deg(asin(sin(theta) * TIBIA / (desiredHeight + 0.0001))); //degrees
  tibiaAngleVal = 190 - (rad2deg(theta) + femurAngleVal * -1 + Y + X);
  switch (leg)
  {
    case 0:
      femurAngle0 = femurAngleVal;
      tibiaAngle0 = tibiaAngleVal;
      break;
    case 1:
      femurAngle1 = femurAngleVal;
      tibiaAngle1 = tibiaAngleVal;
      break;
    case 2:
      femurAngle2 = femurAngleVal;
      tibiaAngle2 = tibiaAngleVal;
      break;
    case 3:
      femurAngle3 = femurAngleVal;
      tibiaAngle3 = tibiaAngleVal;
      break;
  }
}
void stepForth(double desiredHeight, double forwardDist, int leg)
{
  legHeight(sqrt(sq(desiredHeight) + sq(forwardDist)), leg);
  switch (leg)
  {
    case 0:
      femurAngle0 += rad2deg(atan(forwardDist / desiredHeight));
      tibiaAngle0 += rad2deg(atan(forwardDist / desiredHeight));
      break;
    case 1:
      femurAngle1 += rad2deg(atan(forwardDist / desiredHeight));
      tibiaAngle1 += rad2deg(atan(forwardDist / desiredHeight));
      break;
    case 2:
      femurAngle2 += rad2deg(atan(forwardDist / desiredHeight));
      tibiaAngle2 += rad2deg(atan(forwardDist / desiredHeight));
      break;
    case 3:
      femurAngle3 += rad2deg(atan(forwardDist / desiredHeight));
      tibiaAngle3 += rad2deg(atan(forwardDist / desiredHeight));
      break;
  }
}
void sideStep(double oldLegLength, double forwardDist, double latDist,  int leg)
{
  latDist += 39.35;//40.60;//
  oldLegLength -= 13.1;
  abductorAngleVal = rad2deg(atan(latDist / oldLegLength)) - rad2deg(atan(39.35 / oldLegLength)); //40.60
  Serial.println(abductorAngleVal);
  double hypotenuse = sqrt(sq(latDist) + sq(oldLegLength));
  double phi = rad2deg(asin(sin(deg2rad(ABDUCTOR_LEG_ANGLE)) * ABDUCTOR_OFFSET / hypotenuse)); 
  double newLegLength = hypotenuse * sin(deg2rad(180 - phi - ABDUCTOR_LEG_ANGLE)) / sin(deg2rad(ABDUCTOR_LEG_ANGLE));
  Serial.println(newLegLength);
  if (forwardDist == 0)
    legHeight(newLegLength, leg);
  else
    stepForth(newLegLength, forwardDist, leg);

  switch (leg)
  {
    case 0:
      abductorAngle0 = abductorAngleVal;
      break;
    case 1:
      abductorAngle1 = abductorAngleVal;
      break;
    case 2:
      abductorAngle2 = abductorAngleVal;
      break;
    case 3:
      abductorAngle3 = abductorAngleVal;
      break;
  }
}
void verticalAbductors()
{
  //VERTICAL ABDUCTORS
  pca9685.setPWM(aSER0, 0, VERTICALABDUCTOR02 - 4);
  pca9685.setPWM(aSER1, 0, 693 - VERTICALABDUCTOR02);
  pca9685.setPWM(aSER2, 0, VERTICALABDUCTOR02 + 10);
  pca9685.setPWM(aSER3, 0, 686 - VERTICALABDUCTOR02);
}

void defaultPosition()
{
  //VERTICAL ABDUCTORS
  pca9685.setPWM(aSER0, 0, VERTICALABDUCTOR02 - 4);
  pca9685.setPWM(aSER1, 0, 693 - VERTICALABDUCTOR02);
  pca9685.setPWM(aSER2, 0, VERTICALABDUCTOR02 + 10);
  pca9685.setPWM(aSER3, 0, 686 - VERTICALABDUCTOR02);

  //PARALLEL FEMURS
  pca9685.setPWM(fSER0, 0, SERVOMIN2 + 47 * fRATIO0);
  pca9685.setPWM(fSER1, 0, SERVOMIN2 + 45 * fRATIO1);
  pca9685.setPWM(fSER2, 0, SERVOMAX2 - 48 * fRATIO2);
  pca9685.setPWM(fSER3, 0, SERVOMAX2 - 47 * fRATIO3);

  //KNEES IN VERTICAL POSITION
  pca9685.setPWM(tSER0, 0, SERVOMIN2 + 20);
  pca9685.setPWM(tSER1, 0, SERVOMIN2 + 15);
  pca9685.setPWM(tSER2, 0, SERVOMAX2 - 13);
  pca9685.setPWM(tSER3, 0, SERVOMAX2 - 13);
}
double rad2deg(double rad)
{
  return rad * 57296 / 1000;
}
double deg2rad(double deg)
{
  return deg * 1000 / 57296;
}
